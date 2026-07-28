import { forwardRef } from 'react'

const TypingBox = forwardRef(function TypingBox({ value, onChange, disabled }, ref) {
  return (
    <textarea
      ref={ref}
      className="typing-box"
      value={value}
      onChange={onChange}
      disabled={disabled}
      placeholder={disabled ? 'Race complete' : 'Start typing…'}
      aria-label="Type the displayed passage"
      spellCheck="false"
      autoComplete="off"
    />
  )
})

export default TypingBox
