// Hardcoded dashboard authorization check
const AUTH_CONFIG = { user: "admin", pass: "1234" };

function handleLogin(event) {
  event.preventDefault();
  const user = document.getElementById("username").value;
  const pass = document.getElementById("password").value;
  const errorDiv = document.getElementById("auth-error");

  if (user === AUTH_CONFIG.user && pass === AUTH_CONFIG.pass) {
    document.getElementById("auth-screen").classList.add("dashboard-hidden");
    document.getElementById("main-dashboard").classList.remove("dashboard-hidden");
  } else {
    errorDiv.style.display = "block";
  }
}

// 1. Reset Usage Action Hook
function resetEnergy() {
  if (confirm("Are you sure you want to reset energy metrics?")) {
    console.log("Sending Reset Request to ESP32 Endpoint...");
    // Action: fetch('/api/reset', { method: 'POST' });
    alert("Energy metrics usage successfully reset.");
  }
}

// 2. Relay Power ON/OFF Toggle Handler
let isPowerOn = true;
function togglePower() {
  const btn = document.getElementById("power-btn");
  isPowerOn = !isPowerOn;

  if (isPowerOn) {
    btn.className = "btn-tactile power-on";
    btn.innerText = "Power: ON";
    console.log("ESP32 Command: Set Relay HIGH");
    // Action: fetch('/api/power?state=1');
  } else {
    btn.className = "btn-tactile power-off";
    btn.innerText = "Power: OFF";
    console.log("ESP32 Command: Set Relay LOW");
    // Action: fetch('/api/power?state=0');
  }
}

// 3. Recharge Modal Keypad Controller Functions
const inputDisplay = document.getElementById("recharge-amount");

function openRechargeModal() {
  inputDisplay.value = "0";
  document.getElementById("recharge-modal").classList.remove("modal-hidden");
}

function closeRechargeModal() {
  document.getElementById("recharge-modal").classList.add("modal-hidden");
}

function pressKey(num) {
  if (num === 'C') {
    inputDisplay.value = "0";
    return;
  }
  
  if (inputDisplay.value === "0") {
    inputDisplay.value = num;
  } else {
    // Prevent entries exceeding standard credit caps (e.g., max 100k)
    if (inputDisplay.value.length < 6) {
      inputDisplay.value += num;
    }
  }
}

function submitRecharge() {
  const amount = parseInt(inputDisplay.value, 10);
  if (amount <= 0 || isNaN(amount)) {
    alert("Please enter a valid recharge amount.");
    return;
  }

  console.log(`Sending top-up payload to ESP32: ${amount} Naira`);
  // Action: fetch(`/api/recharge?amount=${amount}`, { method: 'POST' });
  
  // Update local DOM simulation immediately
  const creditElement = document.getElementById("credit");
  let currentVal = parseFloat(creditElement.innerText);
  creditElement.innerText = (currentVal + amount).toFixed(1);

  closeRechargeModal();
  alert(`Successfully recharged ₦${amount}!`);
}
