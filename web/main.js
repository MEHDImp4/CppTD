const API_URL = import.meta.env.VITE_SUPABASE_URL + '/functions/v1/weather-api';
const API_KEY = import.meta.env.VITE_SUPABASE_ANON_KEY;

const headers = {
  'Content-Type': 'application/json',
  'Authorization': `Bearer ${API_KEY}`,
  'apikey': API_KEY,
};

let measurements = [];

async function fetchMeasurements() {
  try {
    const response = await fetch(`${API_URL}/measurements`, { headers });
    if (!response.ok) throw new Error('Erreur de chargement');
    measurements = await response.json();
    renderMeasurements();
    updateCount();
  } catch (error) {
    showToast('Erreur lors du chargement des mesures', 'error');
    console.error(error);
  }
}

async function fetchAnalysis() {
  try {
    const response = await fetch(`${API_URL}/analysis`, { headers });
    if (!response.ok) throw new Error('Erreur de chargement');
    const data = await response.json();

    document.getElementById('avg-temp').textContent = `${data.avgTemp}°C`;
    document.getElementById('min-temp').textContent = `${data.minTemp}°C`;
    document.getElementById('max-temp').textContent = `${data.maxTemp}°C`;
    document.getElementById('avg-humidity').textContent = `${data.avgHumidity}%`;
    document.getElementById('avg-wind').textContent = `${data.avgWindSpeed} km/h`;
    document.getElementById('trend').textContent = data.trend;
  } catch (error) {
    showToast('Erreur lors du chargement des analyses', 'error');
    console.error(error);
  }
}

async function addMeasurement(measurement) {
  try {
    const response = await fetch(`${API_URL}/measurements`, {
      method: 'POST',
      headers,
      body: JSON.stringify(measurement),
    });

    if (!response.ok) throw new Error('Erreur d\'ajout');

    showToast('Mesure ajoutée avec succès', 'success');
    await fetchMeasurements();
    await fetchAnalysis();
  } catch (error) {
    showToast('Erreur lors de l\'ajout de la mesure', 'error');
    console.error(error);
  }
}

async function deleteMeasurement(id) {
  if (!confirm('Voulez-vous vraiment supprimer cette mesure ?')) return;

  try {
    const response = await fetch(`${API_URL}/measurements/${id}`, {
      method: 'DELETE',
      headers,
    });

    if (!response.ok) throw new Error('Erreur de suppression');

    showToast('Mesure supprimée', 'success');
    await fetchMeasurements();
    await fetchAnalysis();
  } catch (error) {
    showToast('Erreur lors de la suppression', 'error');
    console.error(error);
  }
}

function renderMeasurements() {
  const container = document.getElementById('measurements-list');

  if (measurements.length === 0) {
    container.innerHTML = `
      <div class="empty-state">
        <p>📭</p>
        <p>Aucune mesure enregistrée</p>
      </div>
    `;
    return;
  }

  container.innerHTML = measurements.map(m => `
    <div class="measurement-item">
      <div class="measurement-info">
        <div class="measurement-field">
          <span class="field-label">Température</span>
          <span class="field-value">${m.temperature}°C</span>
        </div>
        <div class="measurement-field">
          <span class="field-label">Humidité</span>
          <span class="field-value">${m.humidity}%</span>
        </div>
        <div class="measurement-field">
          <span class="field-label">Vitesse vent</span>
          <span class="field-value">${m.wind_speed} km/h</span>
        </div>
        <div class="measurement-field">
          <span class="field-label">Direction</span>
          <span class="field-value">${m.wind_direction}</span>
        </div>
        <div class="measurement-date">
          📅 ${new Date(m.date_time).toLocaleString('fr-FR', {
            year: 'numeric',
            month: 'long',
            day: 'numeric',
            hour: '2-digit',
            minute: '2-digit'
          })}
        </div>
      </div>
      <button class="btn btn-danger" onclick="deleteMeasurement('${m.id}')">
        Supprimer
      </button>
    </div>
  `).join('');
}

function updateCount() {
  document.getElementById('count').textContent = measurements.length;
}

function showToast(message, type = 'success') {
  const toast = document.getElementById('toast');
  toast.textContent = message;
  toast.className = `toast ${type} show`;

  setTimeout(() => {
    toast.classList.remove('show');
  }, 3000);
}

function setCurrentDateTime() {
  const now = new Date();
  const offset = now.getTimezoneOffset() * 60000;
  const localISOTime = new Date(now - offset).toISOString().slice(0, 16);
  document.getElementById('dateTime').value = localISOTime;
}

document.getElementById('add-form').addEventListener('submit', async (e) => {
  e.preventDefault();

  const measurement = {
    temperature: parseFloat(document.getElementById('temperature').value),
    humidity: parseFloat(document.getElementById('humidity').value),
    wind_speed: parseFloat(document.getElementById('windSpeed').value),
    wind_direction: document.getElementById('windDirection').value,
    date_time: new Date(document.getElementById('dateTime').value).toISOString(),
  };

  await addMeasurement(measurement);
  e.target.reset();
  setCurrentDateTime();
});

window.deleteMeasurement = deleteMeasurement;

setCurrentDateTime();
fetchMeasurements();
fetchAnalysis();
