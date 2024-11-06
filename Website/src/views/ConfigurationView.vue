<template>
	<div class="settings-container">
		<LoadingOverlay :is-visible="isLoading" :message="loadingMessage" />
		<h1 class="settings-title">
			<LayoutDashboard class="settings-icon" />
			Devices Configuration
		</h1>

		<!-- Lista urządzeń -->
		<div class="settings-grid">
			<div class="settings-section">
				<h2 class="section-title">
					<CircuitBoard class="section-icon" />
					Configured Devices
				</h2>

				<div class="devices-list">
					<div v-for="device in devices" :key="device.DeviceID" class="device-card">
						<div class="device-info">
							<CircuitBoard class="device-icon" />
							<div class="device-details">
								<span class="device-name">{{ device.Description }}</span>
								<span class="device-type">{{ getDeviceTypeName(device.DeviceTypeID) }}</span>
							</div>
						</div>
						<button class="delete-button" @click="deleteDevice(device.DeviceID)">
							<Trash2 class="delete-icon" />
						</button>
					</div>
				</div>
			</div>

			<!-- Formularz dodawania nowego urządzenia -->
			<div class="settings-section">
				<h2 class="section-title">
					<PlusCircle class="section-icon" />
					Add New Device
				</h2>

				<div class="settings-form">
					<div class="form-group">
						<label for="deviceType">Device Type</label>
						<div class="input-with-icon">
							<CircuitBoard class="input-icon" />
							<select id="deviceType" v-model="newDevice.typeId" class="select-input">
								<option value="">Select device type</option>
								<option v-for="type in deviceTypes" :key="type.TypeID" :value="type.TypeID">
									{{ type.Name }}
								</option>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="deviceDescription">Description</label>
						<div class="input-with-icon">
							<FileText class="input-icon" />
							<input type="text" id="deviceDescription" v-model="newDevice.description"
								placeholder="Enter device description">
						</div>
					</div>

					<div class="settings-actions">
						<button class="action-button save" @click="addDevice">
							<Save class="button-icon" />
							Add Device
						</button>
					</div>
				</div>
			</div>
		</div>
	</div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import axios from 'axios'
import LoadingOverlay from '../components/loadingOverlay.vue'
import {
	LayoutDashboard,
	CircuitBoard,
	PlusCircle,
	Save,
	Trash2,
	FileText
} from 'lucide-vue-next'

interface DeviceType {
	Name: string
	Signals: Array<{
		IsAnalog: boolean
		IsSteerable: boolean
		Name: string
		Unit: string
	}>
	TypeID: number
}

interface Device {
	Description: string
	DeviceID: number
	DeviceTypeID: number
}

const isLoading = ref(false)
const loadingMessage = ref('')
const devices = ref<Device[]>([])
const deviceTypes = ref<DeviceType[]>([])
const newDevice = ref({
	typeId: '',
	description: ''
})

// Pobieranie typów urządzeń
const fetchDeviceTypes = async () => {
	try {
		const response = await axios.get('http://localhost:9999/api/devices/types/get')
		deviceTypes.value = response.data
	} catch (error) {
		console.error('Error fetching device types:', error)
	}
}

// Pobieranie listy urządzeń
const fetchDevices = async () => {
	try {
		const response = await axios.get('http://localhost:9999/api/devices/get')
		devices.value = response.data
	} catch (error) {
		console.error('Error fetching devices:', error)
	}
}

// Dodawanie nowego urządzenia
const addDevice = async () => {
	if (!newDevice.value.typeId || !newDevice.value.description) {
		alert('Please fill in all fields')
		return
	}

	isLoading.value = true
	loadingMessage.value = 'Adding new device...'

	try {
		// TODO: Implement actual API call
		// await axios.post('http://localhost:9999/api/devices/add', {
		//   deviceTypeId: newDevice.value.typeId,
		//   description: newDevice.value.description
		// })

		await fetchDevices()
		newDevice.value = {
			typeId: '',
			description: ''
		}
	} catch (error) {
		console.error('Error adding device:', error)
	} finally {
		isLoading.value = false
	}
}

// Usuwanie urządzenia
const deleteDevice = async (deviceId: number) => {
	if (!confirm('Are you sure you want to delete this device?')) {
		return
	}

	isLoading.value = true
	loadingMessage.value = 'Deleting device...'

	try {
		// TODO: Implement actual API call
		// await axios.delete(`http://localhost:9999/api/devices/delete/${deviceId}`)
		await fetchDevices()
	} catch (error) {
		console.error('Error deleting device:', error)
	} finally {
		isLoading.value = false
	}
}

// Pomocnicza funkcja do pobrania nazwy typu urządzenia
const getDeviceTypeName = (typeId: number): string => {
	const deviceType = deviceTypes.value.find(type => type.TypeID === typeId)
	return deviceType?.Name || 'Unknown Type'
}

onMounted(async () => {
	isLoading.value = true
	loadingMessage.value = 'Loading configuration...'

	try {
		await Promise.all([
			fetchDeviceTypes(),
			fetchDevices()
		])
	} catch (error) {
		console.error('Error during initialization:', error)
	} finally {
		isLoading.value = false
	}
})
</script>

<style scoped>
.settings-container {
	padding: 20px;
	max-width: 1200px;
	margin: 0 auto;
}

.settings-title {
	display: flex;
	align-items: center;
	gap: 12px;
	color: #2c3e50;
	margin-bottom: 30px;
	font-size: 1.8rem;
}

.settings-icon {
	width: 28px;
	height: 28px;
	color: #3498db;
}

.settings-grid {
	display: grid;
	grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
	gap: 24px;
}

.settings-section {
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
	margin-bottom: 20px;
}

.section-icon {
	width: 20px;
	height: 20px;
	color: #3498db;
}

.devices-list {
	display: flex;
	flex-direction: column;
	gap: 12px;
}

.device-card {
	display: flex;
	justify-content: space-between;
	align-items: center;
	padding: 12px;
	background: #f8fafc;
	border-radius: 8px;
	transition: all 0.3s;
	border-left: 3px solid transparent;
}

.device-card:hover {
	background: #f1f5f9;
	transform: translateX(4px);
}

.device-info {
	display: flex;
	align-items: center;
	gap: 12px;
}

.device-icon {
	width: 18px;
	height: 18px;
	color: #3498db;
}

.device-details {
	display: flex;
	flex-direction: column;
}

.device-name {
	font-size: 0.9rem;
	color: #334155;
	font-weight: 500;
}

.device-type {
	font-size: 0.8rem;
	color: #64748b;
}

.delete-button {
	background: none;
	border: none;
	padding: 8px;
	cursor: pointer;
	display: flex;
	align-items: center;
	justify-content: center;
	border-radius: 6px;
	transition: all 0.3s;
}

.delete-button:hover {
	background: rgba(239, 68, 68, 0.1);
}

.delete-icon {
	width: 18px;
	height: 18px;
	color: #ef4444;
}

.settings-form {
	display: flex;
	flex-direction: column;
	gap: 16px;
}

.form-group {
	display: flex;
	flex-direction: column;
	gap: 8px;
}

.form-group label {
	color: #2c3e50;
	font-weight: 500;
	font-size: 0.9rem;
}

.input-with-icon {
	position: relative;
	display: flex;
	align-items: center;
}

.input-icon {
	position: absolute;
	left: 12px;
	width: 18px;
	height: 18px;
	color: #94a3b8;
}

input,
.select-input {
	width: 100%;
	padding: 10px 12px 10px 40px;
	border: 1px solid #e2e8f0;
	border-radius: 8px;
	font-size: 0.9rem;
	transition: all 0.3s;
	background-color: white;
}

input:focus,
.select-input:focus {
	outline: none;
	border-color: #3498db;
	box-shadow: 0 0 0 3px rgba(52, 152, 219, 0.1);
}

input:hover,
.select-input:hover {
	border-color: #3498db;
}

.settings-actions {
	display: flex;
	justify-content: flex-end;
	margin-top: 20px;
}

.action-button {
	display: flex;
	align-items: center;
	gap: 8px;
	padding: 10px 20px;
	border-radius: 8px;
	font-weight: 500;
	cursor: pointer;
	transition: all 0.3s;
	border: none;
}

.action-button.save {
	background-color: #3498db;
	color: white;
}

.action-button.save:hover {
	background-color: #2980b9;
}

.button-icon {
	width: 18px;
	height: 18px;
}

@media (max-width: 768px) {
	.settings-grid {
		grid-template-columns: 1fr;
	}

	.settings-container {
		padding: 10px;
	}

	.settings-title {
		font-size: 1.5rem;
	}

	.device-card {
		padding: 10px;
	}

	.device-name {
		font-size: 0.85rem;
	}
}
</style>