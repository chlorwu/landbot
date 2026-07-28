import { StrictMode } from 'react'
import { createRoot } from 'react-dom/client'
import RaceApp from './RaceApp'

createRoot(document.getElementById('root')).render(
  <StrictMode>
    <RaceApp />
  </StrictMode>,
)
