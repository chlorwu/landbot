import { useState, useEffect } from 'react'
import reactLogo from './assets/react.svg'
import viteLogo from './assets/vite.svg'
import heroImg from './assets/hero.png'
import './App.css'
import { pushDataWithFirebaseId } from './firebase'


function App() {
  const [count, setCount] = useState(0)
  const [id, setId] = useState(localStorage.getItem("landbotUUID"))
  async function updateScore(){
    setCount((count) => count + 1)
    await pushDataWithFirebaseId(count, id )
  }
  function handleUserID() {
    let storedId = localStorage.getItem("landbotUUID");

    if (!storedId) {
      storedId = crypto.randomUUID();
      localStorage.setItem("landbotUUID", storedId);
      console.log("Created UUID:", storedId);
    }

    setId(storedId);
  }
  
  
  useEffect(() => {
    handleUserID();
  }, []);
  return (
    <>
    
  <section id="center">
        <div className="hero">
          <img src={heroImg} className="base" width="170" height="179" alt="" />
          <img src={reactLogo} className="framework" alt="React logo" />
          <img src={viteLogo} className="vite" alt="Vite logo" />
        </div>
        <div>
          <h1>Get started</h1>
          <p>
            Edit <code>src/App.jsx</code> and save to test <code>HMR</code>
          </p>
        </div>
        <button
          type="button"
          className="counter"

          onClick={updateScore}
        >
          Count is {count}
        </button>
      </section>


    </>
  )
}

export default App
