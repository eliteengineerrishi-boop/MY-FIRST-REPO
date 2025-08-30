let countdown;
let timerDisplay = document.getElementById("timer");
let message = document.getElementById("message");

function startTimer(seconds) {
  clearInterval(countdown);
  const now = Date.now();
  const then = now + seconds * 1000;

  countdown = setInterval(() => {
    const secondsLeft = Math.round((then - Date.now()) / 1000);
    if (secondsLeft < 0) {
      clearInterval(countdown);
      timerDisplay.textContent = "00:00";
      message.textContent = "🥚 Done! Enjoy your egg!";
      return;
    }
    const min = Math.floor(secondsLeft / 60);
    const sec = secondsLeft % 60;
    timerDisplay.textContent = `${min.toString().padStart(2, '0')}:${sec.toString().padStart(2, '0')}`;
    message.textContent = "";
  }, 1000);
}
