import { styled } from 'stitches.config'

export const ColoredSvg = styled('svg', {
  fill: '$gray600',
  '&:hover': {
    fill: '$gray400',
    cursor: 'pointer',
  },
})

interface IconProps {
  onClick?(): void
}
export const HomeIcon = (props: IconProps) => {
  return (
    <ColoredSvg
      width={35}
      height={40}
      viewBox="0 0 30 33"
      xmlns="http://www.w3.org/2000/svg"
      onClick={props.onClick}
    >
      <path d="M27 18.039L16 9.501 5 18.039V14.56l11-8.54 11 8.538v3.481zm-2.75-.31v8.251h-5.5v-5.5h-5.5v5.5h-5.5v-8.25L16 11.543l8.25 6.186z" />
    </ColoredSvg>
  )
}
export const ShareIcon = (props: IconProps) => {
  return (
    <ColoredSvg
      width={26}
      height={26}
      viewBox="0 0 1920 1920"
      xmlns="http://www.w3.org/2000/svg"
      onClick={props.onClick}
    >
      <path
        d="M1513.827 1278.28c-96.772 0-182.634 43.765-241.415 111.531l-564.2-325.737c11.807-33.498 19.508-69.049 19.508-106.654 0-34.91-7.06-68.022-17.327-99.595l563.815-325.48c58.782 66.482 143.746 109.35 239.619 109.35 177.243 0 320.86-143.618 320.86-320.86 0-177.244-143.617-320.86-320.86-320.86-177.243 0-320.86 143.616-320.86 320.86 0 35.165 7.059 68.407 17.454 99.98l-563.686 325.48C587.953 679.554 502.86 636.56 406.86 636.56 229.617 636.56 86 780.177 86 957.42c0 177.243 143.617 320.86 320.86 320.86 93.434 0 176.601-40.428 235.254-104.215l567.537 327.662c-9.882 30.803-16.684 63.145-16.684 97.413 0 177.243 143.617 320.86 320.86 320.86 177.243 0 320.86-143.617 320.86-320.86 0-177.243-143.617-320.86-320.86-320.86"
        fill-rule="evenodd"
      />
    </ColoredSvg>
  )
}
export const DiceIcon = (props: IconProps) => {
  return (
    <ColoredSvg
      height={30}
      width={30}
      version="1.1"
      id="_x32_"
      onClick={props.onClick}
      xmlns="http://www.w3.org/2000/svg"
      viewBox="0 0 512 512"
    >
      <g>
        <path
          className="st0"
          d="M103.958,265.785c12.574,5.162,23.463-1.538,24.316-14.964c0.852-13.427-8.654-28.492-21.221-33.654
		c-12.568-5.156-23.457,1.537-24.31,14.964C81.891,245.557,91.397,260.629,103.958,265.785z"
        />
        <path
          className="st0"
          d="M202.141,421.758c12.574,5.163,23.457-1.544,24.309-14.964c0.852-13.427-8.647-28.499-21.214-33.655
		c-12.574-5.156-23.457,1.537-24.316,14.964C180.067,401.531,189.574,416.602,202.141,421.758z"
        />
        <path
          className="st0"
          d="M449.531,105.602L288.463,8.989C278.473,2.994,267.235,0,256.01,0c-11.238,0-22.483,2.994-32.466,8.989
		L62.475,105.602c-19.012,11.406-30.647,31.949-30.647,54.117v192.562c0,22.168,11.634,42.711,30.647,54.117l161.069,96.613
		c9.982,5.995,21.228,8.989,32.466,8.989c11.225,0,22.464-3.001,32.453-8.989l161.069-96.613
		c19.012-11.406,30.64-31.95,30.64-54.117V159.719C480.171,137.551,468.544,117.008,449.531,105.602z M250.599,492.733
		c-6.028-0.745-11.929-2.719-17.32-5.949L72.21,390.164c-13.306-7.982-21.456-22.362-21.456-37.884V159.719
		c0-6.022,1.236-11.862,3.518-17.234l196.328,117.767V492.733z M59.669,133.114c3.363-4.464,7.593-8.318,12.541-11.286
		l161.069-96.613c6.996-4.196,14.85-6.29,22.731-6.29c7.869,0,15.723,2.095,22.718,6.29l161.069,96.613
		c4.941,2.968,9.171,6.821,12.541,11.286L256.01,250.881L59.669,133.114z M461.253,352.281c0,15.521-8.15,29.901-21.456,37.884
		l-161.069,96.62c-5.397,3.23-11.292,5.21-17.32,5.949v-232.48l196.328-117.767c2.282,5.371,3.518,11.212,3.518,17.234V352.281z"
        />
        <path
          className="st0"
          d="M275.519,44.678c-9.829-7.27-26.021-7.459-36.172-0.403c-10.137,7.049-10.392,18.677-0.564,25.948
		c9.828,7.277,26.021,7.459,36.158,0.41C285.093,63.576,285.348,51.961,275.519,44.678z"
        />
        <path
          className="st0"
          d="M382.344,115.773c-9.829-7.278-26.022-7.459-36.159-0.41c-10.151,7.049-10.406,18.677-0.578,25.948
		c9.829,7.277,26.022,7.459,36.159,0.416C391.917,134.664,392.172,123.057,382.344,115.773z"
        />
        <path
          className="st0"
          d="M165.621,113.564c-9.829-7.278-26.022-7.459-36.172-0.41c-10.137,7.056-10.393,18.67-0.57,25.948
		c9.834,7.284,26.028,7.459,36.165,0.41C175.194,132.456,175.449,120.842,165.621,113.564z"
        />
        <path
          className="st0"
          d="M272.444,184.653c-9.828-7.278-26.021-7.459-36.172-0.41c-10.137,7.056-10.392,18.67-0.564,25.948
		c9.829,7.284,26.022,7.465,36.172,0.409C282.018,203.551,282.273,191.93,272.444,184.653z"
        />
        <path
          className="st0"
          d="M418.341,207.901c-11.158,6.082-20.207,20.053-20.207,31.211c0,11.151,9.05,15.266,20.207,9.19
		c11.144-6.082,20.194-20.06,20.194-31.21C438.535,205.934,429.485,201.819,418.341,207.901z"
        />
        <path
          className="st0"
          d="M305.555,269.369c-11.158,6.083-20.194,20.06-20.194,31.211c0,11.158,9.036,15.273,20.194,9.191
		c11.158-6.083,20.194-20.053,20.194-31.211C325.75,267.409,316.713,263.294,305.555,269.369z"
        />
        <path
          className="st0"
          d="M418.341,320.68c-11.158,6.082-20.207,20.053-20.207,31.211c0,11.15,9.05,15.266,20.207,9.19
		c11.144-6.082,20.194-20.06,20.194-31.211C438.535,318.714,429.485,314.598,418.341,320.68z"
        />
        <path
          className="st0"
          d="M305.555,382.148c-11.158,6.083-20.194,20.06-20.194,31.211c0,11.158,9.036,15.273,20.194,9.191
		c11.158-6.083,20.194-20.053,20.194-31.211C325.75,380.188,316.713,376.073,305.555,382.148z"
        />
        <path
          className="st0"
          d="M361.948,295.028c-11.158,6.076-20.208,20.053-20.208,31.204c0,11.158,9.05,15.273,20.208,9.19
		c11.158-6.082,20.194-20.052,20.194-31.21C382.142,293.061,373.106,288.946,361.948,295.028z"
        />
      </g>
    </ColoredSvg>
  )
}
export const HelpIcon = (props: IconProps) => {
  return (
    <ColoredSvg
      width={36}
      height={36}
      onClick={props.onClick}
      viewBox="0 0 24 24"
      fill="none"
      xmlns="http://www.w3.org/2000/svg"
    >
      <path d="M12 3C7.04 3 3 7.04 3 12C3 16.96 7.04 21 12 21C16.96 21 21 16.96 21 12C21 7.04 16.96 3 12 3ZM12 19.5C7.86 19.5 4.5 16.14 4.5 12C4.5 7.86 7.86 4.5 12 4.5C16.14 4.5 19.5 7.86 19.5 12C19.5 16.14 16.14 19.5 12 19.5ZM14.3 7.7C14.91 8.31 15.25 9.13 15.25 10C15.25 10.87 14.91 11.68 14.3 12.3C13.87 12.73 13.33 13.03 12.75 13.16V13.5C12.75 13.91 12.41 14.25 12 14.25C11.59 14.25 11.25 13.91 11.25 13.5V12.5C11.25 12.09 11.59 11.75 12 11.75C12.47 11.75 12.91 11.57 13.24 11.24C13.57 10.91 13.75 10.47 13.75 10C13.75 9.53 13.57 9.09 13.24 8.76C12.58 8.1 11.43 8.1 10.77 8.76C10.44 9.09 10.26 9.53 10.26 10C10.26 10.41 9.92 10.75 9.51 10.75C9.1 10.75 8.76 10.41 8.76 10C8.76 9.13 9.1 8.32 9.71 7.7C10.94 6.47 13.08 6.47 14.31 7.7H14.3ZM13 16.25C13 16.8 12.55 17.25 12 17.25C11.45 17.25 11 16.8 11 16.25C11 15.7 11.45 15.25 12 15.25C12.55 15.25 13 15.7 13 16.25Z" />
    </ColoredSvg>
  )
}
