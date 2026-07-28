import Timer from './Timer'

export default function Stats({ wpm, accuracy, score, timeLeft, position }) {
  const items = [
    ['Time left', <Timer key="timer" seconds={timeLeft} />],
    ['WPM', wpm],
    ['Accuracy', `${accuracy}%`],
    ['Score', score],
    ['Position', `#${position}`],
  ]

  return <section className="race-stats" aria-label="Race statistics">
    {items.map(([label, value]) => (
      <div className="stat" key={label}>
        <span>{label}</span>
        <strong>{value}</strong>
      </div>
    ))}
  </section>
}
