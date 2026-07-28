import { useEffect, useMemo, useRef, useState } from 'react'
import { onValue, ref } from 'firebase/database'
import Stats from './components/Stats'
import { passages } from './data/passages'
import { database, pushDataWithFirebaseId } from '../firebase'
import './race.css'

const ROUND_SECONDS = 60

function getPlayerId() {
  let id = localStorage.getItem('landbotUUID')
  if (!id) {
    id = crypto.randomUUID()
    localStorage.setItem('landbotUUID', id)
  }
  return id
}

function RaceTrack({ players, playerId }) {
  const entries = Object.entries(players).sort(([, a], [, b]) => (b.score || 0) - (a.score || 0))
  return <section className="track" aria-label="Live race track">
    <div className="track-lines" />
    {entries.map(([id, player], index) => (
      <div className={`racer ${id === playerId ? 'is-local' : ''}`} key={id} style={{ left: `${Math.min(94, (player.score || 0) / 1.5)}%`, top: `${18 + index * 30}%` }}>
        <span className="car">{id === playerId ? '🤖' : '🏎️'}</span><small>{id === playerId ? 'You' : `Racer ${index + 1}`}</small>
      </div>
    ))}
  </section>
}

export default function RaceApp() {
  const latestScore = useRef(0)
  const [phase, setPhase] = useState('home')
  const [countdown, setCountdown] = useState(3)
  const [playerId] = useState(getPlayerId)
  const [passage, setPassage] = useState('')
  const [text, setText] = useState('')
  const [timeLeft, setTimeLeft] = useState(ROUND_SECONDS)
  const [players, setPlayers] = useState({})

  const correctCharacters = useMemo(() => [...text].filter((character, index) => character === passage[index]).length, [text, passage])
  const elapsed = ROUND_SECONDS - timeLeft
  const wpm = elapsed ? Math.round((correctCharacters / 5) / (elapsed / 60)) : 0
  const accuracy = text.length ? Math.round((correctCharacters / text.length) * 100) : 100
  const score = Math.round(wpm * (accuracy / 100))
  const errors = text.length - correctCharacters
  const position = Object.entries(players).filter(([id, player]) => id !== playerId && (player.score || 0) > score).length + 1

  useEffect(() => {
    latestScore.current = score
  }, [score])

  useEffect(() => {
    const unsubscribe = onValue(ref(database, 'players'), snapshot => setPlayers(snapshot.val() || {}))
    return unsubscribe
  }, [])

  useEffect(() => {
    if (phase !== 'countdown') return undefined
    if (countdown === 0) {
      const goTimer = window.setTimeout(() => setPhase('racing'), 500)
      return () => window.clearTimeout(goTimer)
    }
    const timer = window.setTimeout(() => setCountdown(value => value - 1), 1000)
    return () => window.clearTimeout(timer)
  }, [phase, countdown])

  useEffect(() => {
    if (phase !== 'racing') return undefined
    const timer = window.setTimeout(() => {
      if (timeLeft <= 1) {
        setTimeLeft(0)
        setPhase('finished')
      } else {
        setTimeLeft(value => value - 1)
      }
    }, 1000)
    return () => window.clearTimeout(timer)
  }, [phase, timeLeft])

  useEffect(() => {
    if (phase !== 'racing') return undefined
    const writeScore = () => pushDataWithFirebaseId(latestScore.current, playerId).catch(console.error)
    writeScore()
    const writer = window.setInterval(writeScore, 1000)
    return () => window.clearInterval(writer)
  }, [phase, playerId])

  useEffect(() => {
    if (phase === 'finished') pushDataWithFirebaseId(score, playerId).catch(console.error)
  }, [phase, score, playerId])

  function startRace() {
    setPassage(passages[Math.floor(Math.random() * passages.length)])
    setText('')
    setTimeLeft(ROUND_SECONDS)
    setCountdown(3)
    setPhase('countdown')
  }

  useEffect(() => {
    if (phase !== 'racing') return undefined

    function handleKeyDown(event) {
      if (event.ctrlKey || event.metaKey || event.altKey) return

      if (event.key === 'Backspace') {
        event.preventDefault()
        setText(text.slice(0, -1))
        return
      }

      if (event.key.length !== 1) return
      event.preventDefault()
      const nextValue = text + event.key
      if (nextValue.length > passage.length) return
      setText(nextValue)
      if (nextValue === passage) setPhase('finished')
    }

    window.addEventListener('keydown', handleKeyDown)
    return () => window.removeEventListener('keydown', handleKeyDown)
  }, [phase, passage, text])

  if (phase === 'home') return <main className="race-app landing"><p className="eyebrow">LAND BOT LEAGUE</p><h1>Type. Race. Drive.</h1><p>Every accurate word gives your robot more speed. Out-type the field and lead the race.</p><button className="primary-button" onClick={startRace}>Start race <span>→</span></button></main>
  if (phase === 'countdown') return <main className="race-app countdown"><p>Prepare to launch</p><strong>{countdown || 'GO!'}</strong></main>

  return <main className="race-app" tabIndex="-1">
    <header><div><p className="eyebrow">LAND BOT LEAGUE</p><h1>Typing Grand Prix</h1></div><span className="live"><i /> LIVE</span></header>
    <RaceTrack players={{ ...players, [playerId]: { score } }} playerId={playerId} />
    <Stats wpm={wpm} accuracy={accuracy} score={score} timeLeft={timeLeft} position={position} />
    {phase === 'finished' ? <section className="results"><span>🏁</span><p className="eyebrow">RACE COMPLETE</p><h2>Finished in position #{position}</h2><div><b>{wpm}<small> WPM</small></b><b>{accuracy}<small>% accurate</small></b><b>{score}<small> score</small></b><b>{errors}<small> errors</small></b></div><button className="primary-button" onClick={startRace}>Race again →</button></section> : <section className="passage" aria-label="Passage to type">{[...passage].map((character, index) => <span className={index < text.length ? (text[index] === character ? 'correct' : 'incorrect') : index === text.length ? 'current' : ''} key={`${character}-${index}`}>{character}</span>)}</section>}
  </main>
}
