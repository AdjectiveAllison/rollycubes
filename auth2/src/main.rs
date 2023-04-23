use auth::migrations::*;
use auth::routes::{server_routes, user_routes};
use auth::{Jwt, RouterState};
use axum::http::{HeaderName, HeaderValue, Method};
use axum::{
    routing::{get, post},
    Router,
};
use std::fs;
use std::net::SocketAddr;
use std::sync::Arc;
use tokio_postgres::NoTls;
use tower_http::cors::{AllowHeaders, AllowOrigin, CorsLayer};

#[tokio::main]
async fn main() {
    let manager = bb8_postgres::PostgresConnectionManager::new_from_stringlike(
        "host=localhost user=test password=test",
        NoTls,
    )
    .unwrap();
    let pool = bb8::Pool::builder().build(manager).await.unwrap();

    run_migrations(&mut pool.get().await.unwrap())
        .await
        .expect("can run DB migrations: {}");

    let router_state = RouterState {
        jwt: Arc::new(Jwt {
            private_key: fs::read_to_string("./secrets/.id")
                .expect("Should have been able to read the file"),
            public_key: fs::read_to_string("./secrets/.id.pub")
                .expect("Should have been able to read the file"),
        }),
        pool,
    };

    let cors = CorsLayer::new()
        .allow_methods([Method::GET, Method::POST])
        .allow_credentials(true)
        .allow_headers(AllowHeaders::list([
            HeaderName::from_static("csrf-token"),
            HeaderName::from_static("content-type"),
            HeaderName::from_static("x-access-token"),
        ]))
        .allow_origin(AllowOrigin::list([
            HeaderValue::from_static("https://rollycubes.com"),
            HeaderValue::from_static("https://beta.rollycubes.com"),
            HeaderValue::from_static("https://www.rollycubes.com"),
            HeaderValue::from_static("https://rollycubes.live"),
            HeaderValue::from_static("http://localhost:3000"),
            HeaderValue::from_static("http://localhost:3005"),
        ]));

    tracing_subscriber::fmt::init();

    // build our application with a route
    let user_routes = Router::new()
        .route("/twitch_login_or_register", post(user_routes::login))
        .route("/logout", post(user_routes::logout))
        .route("/refresh_token", get(user_routes::refresh_token))
        .route("/public_key", get(user_routes::public_key))
        .route("/me", get(user_routes::user_self))
        .route("/users/:id", get(user_routes::user_by_id))
        .layer(cors);

    let server_routes = Router::new()
        .route("/add_stats", post(server_routes::add_stats))
        .layer(axum::middleware::from_fn(server_routes::auth_layer));
    // .route("/achievement_progress", post(achievement_progress));

    let app = Router::new()
        .nest("/", user_routes)
        .nest("/server", server_routes)
        .with_state(router_state);

    let addr = SocketAddr::from(([127, 0, 0, 1], 3031));
    tracing::debug!("listening on {}", addr);
    axum::Server::bind(&addr)
        .serve(app.into_make_service())
        .await
        .unwrap();
}