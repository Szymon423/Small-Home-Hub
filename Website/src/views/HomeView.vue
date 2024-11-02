<template>
	<div class="home-container">
	  <h1 class="home-title">
		<Home class="home-icon" />
		Home Automation Center
	  </h1>
  
	  <div class="groups-grid">
		<!-- Iteracja po grupach -->
		<div v-for="group in groups" :key="group.id" class="group-section">
		  <h2 class="section-title">
			<Home class="section-icon" />
			{{ group.name }}
		  </h2>
		  <p class="section-description">{{ group.description }}</p>
		  
		  <div class="signals-list">
			<!-- Iteracja po sygnałach w grupie -->
			<div v-for="signal in group.signals" :key="signal.id" class="signal-card">
			  <div class="signal-info">
				<Activity class="signal-icon" />
				<div class="signal-details">
				  <span class="signal-name">{{ signal.name }}</span>
				  <span class="signal-description">{{ signal.description }}</span>
				</div>
			  </div>
			  <div class="signal-value">
				{{ signal.value }}{{ signal.unit }}
			  </div>
			</div>
		  </div>
		</div>
	  </div>
	</div>
  </template>
  
  <script setup lang="ts">
  import { ref, onMounted } from 'vue'
  import { 
	Home,
	Activity
  } from 'lucide-vue-next'
  
  interface Signal {
	id: string
	name: string
	description: string
	value: string | number
	groupId: string
	unit?: string
  }
  
  interface Group {
	id: string
	name: string
	description: string
	signals?: Signal[]
  }
  
  const groups = ref<Group[]>([])
  
  const fetchData = async () => {
	// Tutaj będzie właściwe zapytanie do API
	// Przykładowe dane:
	groups.value = [
	  {
		id: 'living-room',
		name: 'Living Room',
		description: 'Main living area signals',
		signals: [
		  {
			id: 'temp-living',
			name: 'Room Temperature',
			description: 'Main living room temperature sensor',
			value: 23.5,
			unit: '°C',
			groupId: 'living-room'
		  },
		  {
			id: 'light-living',
			name: 'Main Light',
			description: 'Living room main light switch',
			value: 'ON',
			groupId: 'living-room'
		  }
		]
	  },
	  {
		id: 'bathroom',
		name: 'Bathroom',
		description: 'Bathroom monitoring system',
		signals: [
		  {
			id: 'humid-bath',
			name: 'Humidity Level',
			description: 'Bathroom humidity sensor',
			value: 65,
			unit: '%',
			groupId: 'bathroom'
		  },
		  {
			id: 'vent-bath',
			name: 'Ventilation',
			description: 'Bathroom ventilation status',
			value: 'OFF',
			groupId: 'bathroom'
		  }
		]
	  },
	  {
		id: 'kitchen',
		name: 'Kitchen',
		description: 'Kitchen monitoring system',
		signals: [
		  {
			id: 'temp-kitchen',
			name: 'Temperature',
			description: 'Kitchen temperature sensor',
			value: 22.1,
			unit: '°C',
			groupId: 'kitchen'
		  },
		  {
			id: 'smoke-kitchen',
			name: 'Smoke Detector',
			description: 'Kitchen smoke detection status',
			value: 'OK',
			groupId: 'kitchen'
		  }
		]
	  }
	]
  }
  
  onMounted(() => {
	fetchData()
	// Odświeżanie danych co 5 sekund
	setInterval(fetchData, 5000)
  })
  </script>
  
  <style scoped>
  .home-container {
	padding: 20px;
	max-width: 1200px;
	margin: 0 auto;
  }
  
  .home-title {
	display: flex;
	align-items: center;
	gap: 12px;
	color: #2c3e50;
	margin-bottom: 30px;
	font-size: 1.8rem;
  }
  
  .home-icon {
	width: 28px;
	height: 28px;
	color: #3498db;
  }
  
  .groups-grid {
	display: grid;
	grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
	gap: 24px;
  }
  
  .group-section {
	background: white;
	border-radius: 12px;
	padding: 20px;
	box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
  }
  
  .section-title {
	display: flex;
	align-items: center;
	gap: 8px;
	color: #2c3e50;
	font-size: 1.2rem;
	margin-bottom: 8px;
  }
  
  .section-description {
	color: #64748b;
	font-size: 0.9rem;
	margin-bottom: 20px;
  }
  
  .section-icon {
	width: 20px;
	height: 20px;
	color: #3498db;
  }
  
  .signals-list {
	display: flex;
	flex-direction: column;
	gap: 12px;
  }
  
  .signal-card {
	display: flex;
	justify-content: space-between;
	align-items: center;
	padding: 12px;
	background: #f8fafc;
	border-radius: 8px;
	transition: all 0.3s;
  }
  
  .signal-card:hover {
	background: #f1f5f9;
	transform: translateX(4px);
  }
  
  .signal-info {
	display: flex;
	align-items: center;
	gap: 12px;
  }
  
  .signal-details {
	display: flex;
	flex-direction: column;
  }
  
  .signal-icon {
	width: 18px;
	height: 18px;
	color: #64748b;
  }
  
  .signal-name {
	font-size: 0.9rem;
	color: #334155;
	font-weight: 500;
  }
  
  .signal-description {
	font-size: 0.8rem;
	color: #64748b;
  }
  
  .signal-value {
	font-weight: 600;
	color: #2c3e50;
	font-size: 0.9rem;
	padding: 4px 8px;
	background: #e2e8f0;
	border-radius: 6px;
  }
  
  @media (max-width: 768px) {
	.groups-grid {
	  grid-template-columns: 1fr;
	}
  }
  </style>
  