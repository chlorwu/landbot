export default function Timer({ seconds }) {
  const minutes = Math.floor(seconds / 60)
  const remainder = String(seconds % 60).padStart(2, '0')
  return <span>{minutes}:{remainder}</span>
}
