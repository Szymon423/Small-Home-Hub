<template>
	<div class="settings-container">
		<LoadingOverlay :is-visible="isLoading" :message="loadingMessage" />
		<h1 class="settings-title">
			<LayoutDashboard class="settings-icon" />
			Devices Configuration
		</h1>

		<div class="settings-grid">
			<!-- Lista urządzeń -->
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
						<button class="settings-button" @click="openDeviceSettings(device)">
							<Settings class="settings-icon-btn" />
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

		<!-- Modal z ustawieniami urządzenia -->
		<div class="modal" v-if="selectedDevice" @click.self="closeDeviceSettings">
			<div class="modal-content">
				<div class="modal-header">
					<h2>Device Settings</h2>
					<button class="close-button" @click="closeDeviceSettings">
						<X class="close-icon" />
					</button>
				</div>

				<div class="modal-tabs">
					<button class="tab-button" :class="{ active: activeTab === 'signals' }"
						@click="activeTab = 'signals'">
						Signals
					</button>
					<button class="tab-button" :class="{ active: activeTab === 'connection' }"
						@click="activeTab = 'connection'">
						Connection
					</button>
				</div>

				<div class="modal-body">
					<!-- Zakładka Signals -->
					<div v-if="activeTab === 'signals'" class="tab-content">
						<div class="signals-list">
							<div v-for="signal in deviceSignals" :key="signal.ID" class="signal-item">
								<div class="signal-header">
									<div class="signal-info">
										<span class="signal-name">{{ signal.Name }}</span>
										<span class="signal-description">{{ signal.Description }}</span>
									</div>
									<button class="edit-signal-button" @click="openSignalSettings(signal)">
										<Settings class="edit-icon" />
									</button>
								</div>
								<div class="signal-details">
									<span class="signal-type" :class="signal.DataType === 1 ? 'analog' : 'binary'">
										{{ signal.DataType === 1 ? 'Analog' : 'Binary' }}
									</span>
									<span class="signal-control" :class="signal.IsSteerable ? 'controllable' : 'read-only'">
										{{ signal.IsSteerable ? 'Controllable' : 'Read-only' }}
									</span>
									<span class="signal-archiving" :class="signal.IsArchived ? 'yes' : 'no'">
										{{ signal.IsArchived ? 'Archiving' : 'Not archiving' }}
									</span>
									<div class="signal-unit" v-if="signal.Label.Unit">
										<span>Unit:</span>
										<span class="value">{{ signal.Label.Unit }}</span>
									</div>
								</div>
							</div>
						</div>
					</div>

					<!-- Zakładka Connection -->
					<div v-if="activeTab === 'connection'" class="tab-content">
						<div class="device-properties">
							<div class="form-group">
								<label>Device Name</label>
								<input type="text" v-model="editedDevice.description" class="edit-input"
									placeholder="Enter device name">
							</div>

							<div class="connection-placeholder">
								Connection settings will be implemented later
							</div>

							<div class="danger-zone">
								<h3>Danger Zone</h3>
								<button class="delete-button-modal" @click="confirmDelete">
									<Trash2 class="delete-icon" />
									Delete Device
								</button>
							</div>
						</div>
					</div>
				</div>

				<div class="modal-footer">
					<button class="action-button secondary" @click="closeDeviceSettings">
						Cancel
					</button>
					<button class="action-button save" @click="saveDeviceSettings">
						<Save class="button-icon" />
						Save Changes
					</button>
				</div>
			</div>
		</div>

		<!-- Modal edycji sygnału -->
		<div class="modal" v-if="selectedSignal" @click.self="closeSignalSettings">
			<div class="modal-content signal-edit-modal">
				<div class="modal-header">
					<h2>Edit Signal: {{ selectedSignal.Name }}</h2>
					<button class="close-button" @click="closeSignalSettings">
						<X class="close-icon" />
					</button>
				</div>

				<div class="modal-body">
					<div class="signal-edit-form">
						<div class="form-group">
							<label>Description</label>
							<input type="text" v-model="editedSignal.Description" class="edit-input"
								placeholder="Enter signal description">
						</div>

						<div class="form-group">
							<label>Unit</label>
							<input type="text" v-model="editedSignal.Label.Unit" class="edit-input"
								placeholder="Enter unit">
						</div>

						<div class="form-group">
							<label>Archiving</label>
							<div class="toggle-switch">
								<input type="checkbox" :id="'archive-toggle-' + editedSignal.ID"
									v-model="editedSignal.IsArchived">
								<label :for="'archive-toggle-' + editedSignal.ID"></label>
							</div>
						</div>

						<!-- Sekcja alarmów -->
						<div class="alarm-section" v-if="editedSignal.DataType === 1">
							<h3>Alarm Settings</h3>

							<div class="alarm-group">
								<h4>High Alarm</h4>
								<div class="form-group">
									<div class="toggle-switch">
										<input type="checkbox" :id="'high-alarm-toggle-' + editedSignal.ID"
											v-model="editedSignal.AlarmHigh.Enabled">
										<label :for="'high-alarm-toggle-' + editedSignal.ID"></label>
									</div>
								</div>
								<div class="form-group" v-if="editedSignal.AlarmHigh.Enabled">
									<label>Threshold</label>
									<input type="number" v-model="editedSignal.AlarmHigh.Threshold" class="edit-input"
										step="0.1">
								</div>
							</div>

							<div class="alarm-group">
								<h4>Low Alarm</h4>
								<div class="form-group">
									<div class="toggle-switch">
										<input type="checkbox" :id="'low-alarm-toggle-' + editedSignal.ID"
											v-model="editedSignal.AlarmLow.Enabled">
										<label :for="'low-alarm-toggle-' + editedSignal.ID"></label>
									</div>
								</div>
								<div class="form-group" v-if="editedSignal.AlarmLow.Enabled">
									<label>Threshold</label>
									<input type="number" v-model="editedSignal.AlarmLow.Threshold" class="edit-input"
										step="0.1">
								</div>
							</div>
						</div>
					</div>
				</div>

				<div class="modal-footer">
					<button class="action-button secondary" @click="closeSignalSettings">
						Cancel
					</button>
					<button class="action-button save" @click="saveSignalSettings">
						<Save class="button-icon" />
						Save Changes
					</button>
				</div>
			</div>
		</div>
	</div>
</template>

<script>
import { ref, onMounted, computed } from 'vue'
import axios from 'axios'
import {
	LayoutDashboard,
	CircuitBoard,
	PlusCircle,
	Settings,
	Save,
	X,
	Trash2,
	FileText
} from 'lucide-vue-next'
import LoadingOverlay from '@/components/LoadingOverlay.vue'
import '@/assets/base.css'

export default {
	name: 'DevicesConfiguration',
	components: {
		LayoutDashboard,
		CircuitBoard,
		PlusCircle,
		Settings,
		Save,
		X,
		Trash2,
		FileText,
		LoadingOverlay
	},
	setup() {
		const isLoading = ref(false)
		const loadingMessage = ref('')
		const devices = ref([])
		const deviceTypes = ref([])
		const selectedDevice = ref(null)
		const editedDevice = ref(null)
		const activeTab = ref('signals')
		const deviceSignals = ref([])
		const selectedSignal = ref(null)
		const editedSignal = ref(null)

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

		// Pobieranie sygnałów dla urządzenia
		const fetchDeviceSignals = async (deviceId) => {
			try {
				const response = await axios.get('http://localhost:9999/api/signals/definitions/get')
				const allSignals = response.data
				deviceSignals.value = allSignals.filter(signal => signal.DeviceID === deviceId)
			} catch (error) {
				console.error('Error fetching device signals:', error)
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
				await axios.post('http://localhost:9999/api/devices/add', {
					DeviceTypeID: parseInt(newDevice.value.typeId),
					Description: newDevice.value.description
				})

				await fetchDevices()
				newDevice.value = {
					typeId: '',
					description: ''
				}
			} catch (error) {
				console.error('Error adding device:', error)
				alert('Failed to add device')
			} finally {
				isLoading.value = false
			}
		}

		// Otwieranie ustawień urządzenia
		const openDeviceSettings = async (device) => {
			selectedDevice.value = device
			editedDevice.value = { ...device }
			activeTab.value = 'signals'
			await fetchDeviceSignals(device.DeviceID)
		}

		// Zamykanie ustawień urządzenia
		const closeDeviceSettings = () => {
			selectedDevice.value = null
			editedDevice.value = null
			deviceSignals.value = []
		}

		// Otwieranie edycji sygnału
		const openSignalSettings = (signal) => {
			selectedSignal.value = signal
			editedSignal.value = JSON.parse(JSON.stringify(signal)) // Deep copy
		}

		// Zamykanie edycji sygnału
		const closeSignalSettings = () => {
			selectedSignal.value = null
			editedSignal.value = null
		}

		// Zapisywanie ustawień sygnału
		const saveSignalSettings = async () => {
			isLoading.value = true
			loadingMessage.value = 'Saving signal settings...'

			try {
				await axios.put('http://localhost:9999/api/signals/definitions/update',
					editedSignal.value
				)

				await fetchDeviceSignals(selectedDevice.value.DeviceID)
				closeSignalSettings()
			} catch (error) {
				console.error('Error updating signal:', error)
				alert('Failed to update signal settings')
			} finally {
				isLoading.value = false
			}
		}

		// Zapisywanie ustawień urządzenia
		const saveDeviceSettings = async () => {
			isLoading.value = true
			loadingMessage.value = 'Saving device settings...'

			try {
				await axios.put('http://localhost:9999/api/devices/update',
					editedDevice.value
				)

				await fetchDevices()
				closeDeviceSettings()
			} catch (error) {
				console.error('Error updating device:', error)
				alert('Failed to update device settings')
			} finally {
				isLoading.value = false
			}
		}

		// Potwierdzenie usunięcia urządzenia
		const confirmDelete = () => {
			if (confirm('Are you sure you want to delete this device? This action cannot be undone.')) {
				deleteDevice()
			}
		}

		// Usuwanie urządzenia
		const deleteDevice = async () => {
			isLoading.value = true
			loadingMessage.value = 'Deleting device...'

			try {
				await axios.delete(`http://localhost:9999/api/devices/delete/${selectedDevice.value.DeviceID}`)
				await fetchDevices()
				closeDeviceSettings()
			} catch (error) {
				console.error('Error deleting device:', error)
				alert('Failed to delete device')
			} finally {
				isLoading.value = false
			}
		}

		// Pomocnicza funkcja do pobierania nazwy typu urządzenia
		const getDeviceTypeName = (typeId) => {
			const deviceType = deviceTypes.value.find(type => type.TypeID === typeId)
			return deviceType ? deviceType.Name : 'Unknown Type'
		}

		// Inicjalizacja komponentu
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
				alert('Failed to load initial data')
			} finally {
				isLoading.value = false
			}
		})

		return {
			isLoading,
			loadingMessage,
			devices,
			deviceTypes,
			selectedDevice,
			editedDevice,
			activeTab,
			deviceSignals,
			selectedSignal,
			editedSignal,
			newDevice,
			getDeviceTypeName,
			addDevice,
			openDeviceSettings,
			closeDeviceSettings,
			openSignalSettings,
			closeSignalSettings,
			saveSignalSettings,
			saveDeviceSettings,
			confirmDelete,
			deleteDevice
		}
	}
}
</script>

<style scoped>

/* ----------------- Layout & Container ----------------- */
.settings-container {
	padding: 2rem;
	max-width: 1200px;
	margin: 0 auto;
	position: relative;
}

.settings-grid {
	display: grid;
	grid-template-columns: repeat(auto-fit, minmax(400px, 1fr));
	gap: 2rem;
}

/* ----------------- Headers & Titles ----------------- */
.settings-title {
	display: flex;
	align-items: center;
	gap: 1rem;
	font-size: 2rem;
	margin-bottom: 2rem;
	color: var(--color-text-primary);
}

.section-title {
	display: flex;
	align-items: center;
	gap: 0.5rem;
	font-size: 1.25rem;
	margin-bottom: 1.5rem;
	color: var(--color-text-primary);
}

/* ----------------- Icons ----------------- */
.settings-icon {
	width: 2rem;
	height: 2rem;
}

.section-icon {
	width: 1.5rem;
	height: 1.5rem;
}

.device-icon {
	width: 1.5rem;
	height: 1.5rem;
	color: var(--color-primary);
}

.settings-icon-btn,
.edit-icon,
.close-icon,
.input-icon {
	width: 1.25rem;
	height: 1.25rem;
	color: var(--color-text-secondary);
}

/* ----------------- Sections ----------------- */
.settings-section {
	background: var(--color-background-secondary);
	border-radius: 12px;
	padding: 1.5rem;
	box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

/* ----------------- Device Card ----------------- */
.devices-list {
	display: flex;
	flex-direction: column;
	gap: 1rem;
}

.device-card {
	display: flex;
	justify-content: space-between;
	align-items: center;
	padding: 1rem;
	background: var(--color-background-primary);
	border-radius: 8px;
	border: 1px solid var(--color-border);
	transition: all 0.2s ease;
}

.device-card:hover {
	transform: translateY(-2px);
	box-shadow: 0 4px 6px rgba(0, 0, 0, 0.1);
}

.device-info {
	display: flex;
	align-items: center;
	gap: 1rem;
}

.device-details {
	display: flex;
	flex-direction: column;
}

.device-name {
	font-weight: 500;
	color: var(--color-text-primary);
}

.device-type {
	font-size: 0.875rem;
	color: var(--color-text-secondary);
}

/* ----------------- Forms ----------------- */
.settings-form,
.signal-edit-form {
	display: flex;
	flex-direction: column;
	gap: 1.5rem;
}

.form-group {
	display: flex;
	flex-direction: column;
	gap: 0.5rem;
}

.form-group label {
	font-size: 0.875rem;
	color: var(--color-text-secondary);
}

.input-with-icon {
	position: relative;
	display: flex;
	align-items: center;
}

.input-icon {
	position: absolute;
	left: 0.75rem;
}

.input-with-icon input,
.input-with-icon select,
.edit-input,
.modal input[type="text"],
.modal input[type="number"],
.modal select {
	width: 100%;
	padding: 0.75rem 1rem 0.75rem 2.5rem;
	border: 1px solid var(--color-border);
	border-radius: 6px;
	background: var(--color-background-primary);
	color: var(--color-text-primary);
	font-size: 1rem;
	transition: border-color 0.2s ease, box-shadow 0.2s ease;
}

.edit-input:focus,
.input-with-icon input:focus,
.input-with-icon select:focus {
	outline: none;
	border-color: var(--color-primary);
	box-shadow: 0 0 0 2px var(--color-primary-light);
}

/* ----------------- Buttons ----------------- */
.settings-button,
.edit-signal-button,
.close-button {
	padding: 0.5rem;
	border-radius: 6px;
	background: transparent;
	border: none;
	cursor: pointer;
	display: flex;
	align-items: center;
	justify-content: center;
	transition: all 0.2s ease;
	color: var(--color-text-secondary);
}

.settings-button:hover,
.edit-signal-button:hover,
.close-button:hover {
	background: var(--color-background-secondary, #f3f4f6);
	color: var(--color-primary);
}

.action-button {
	display: flex;
	align-items: center;
	gap: 0.5rem;
	padding: 0.75rem 1.5rem;
	border-radius: 6px;
	font-weight: 500;
	border: none;
	cursor: pointer;
	transition: all 0.2s ease;
}

.action-button.save {
	background: var(--color-primary);
	color: white;
}

.action-button.save:hover {
	background: var(--color-primary-dark);
}

.action-button.secondary {
	background: var(--color-background-secondary);
	color: var(--color-text-primary);
}

.action-button.secondary:hover {
	background: var(--color-background-hover);
}

.delete-button-modal {
	display: flex;
	align-items: center;
	gap: 0.5rem;
	padding: 0.75rem 1.5rem;
	background: var(--color-danger);
	color: white;
	border: none;
	border-radius: 6px;
	cursor: pointer;
	transition: all 0.2s ease;
}

.delete-button-modal:hover {
	background: var(--color-danger-dark);
}

/* ----------------- Modal ----------------- */
.modal {
	position: fixed;
	top: 0;
	left: 0;
	right: 0;
	bottom: 0;
	background: rgba(0, 0, 0, 0.5);
	display: flex;
	justify-content: center;
	align-items: center;
	z-index: 1000;
}

.modal-content {
	background: var(--color-background-primary, #ffffff);
	border-radius: 12px;
	padding: 2rem;
	width: 90%;
	max-width: 600px;
	max-height: 80vh;
	overflow-y: auto;
	box-shadow: 0 4px 6px -1px rgba(0, 0, 0, 0.1), 0 2px 4px -1px rgba(0, 0, 0, 0.06);
}

.signal-edit-modal {
	max-width: 500px;
}

.modal-header {
	display: flex;
	justify-content: space-between;
	align-items: center;
}

.modal-header h2 {
	font-size: 1.5rem;
	font-weight: 600;
	color: var(--color-text-primary, #1f2937);
}

.modal-body {
	padding: 1rem 0;
}

.modal-footer {
	display: flex;
	justify-content: flex-end;
	gap: 1rem;
	margin-top: 2rem;
	padding-top: 1rem;
	border-top: 1px solid var(--color-border);
}

/* ----------------- Tabs ----------------- */
.modal-tabs {
	display: flex;
	gap: 1rem;
	margin-bottom: 1.5rem;
	border-bottom: 1px solid var(--color-border);
}

.tab-button {
	padding: 0.75rem 1.5rem;
	background: none;
	border: none;
	color: var(--color-text-secondary);
	cursor: pointer;
	font-weight: 500;
	border-bottom: 2px solid transparent;
}

.tab-button.active {
	color: var(--color-primary);
	border-bottom-color: var(--color-primary);
}

.tab-button:focus,
.action-button:focus,
.settings-button:focus {
	outline: none;
	box-shadow: 0 0 0 2px var(--color-primary-light);
}

.tab-content {
	background: var(--color-background-primary);
	border-radius: 8px;
	padding: 1rem 0;
}

/* ----------------- Signals ----------------- */
.signals-list {
	display: flex;
	gap: 1rem;
	flex-direction:column;
}

.signal-item {
	background: var(--color-background-primary);
	border: 1px solid var(--color-border);
	border-radius: 8px;
	padding: 1.5rem;
}

.signal-header {
	display: flex;
	justify-content: space-between;
	align-items: flex-start;
	margin-bottom: 0.5rem;
}

.signal-info {
	display: flex;
	flex-direction: column;
}

.signal-name {
	font-weight: 500;
	color: var(--color-text-primary);
	margin-bottom: 0.25rem;
}

.signal-description {
	font-size: 0.875rem;
	color: var(--color-text-secondary);
}

.signal-details {
	display: flex;
	align-items: center;
	gap: 1.5rem;
	margin-top: 0.5rem;
}

.signal-type {
	display: inline-flex;
	align-items: center;
	padding: 0.25rem 0.75rem;
	border-radius: 4px;
	font-size: 0.875rem;
	font-weight: 500;
}

.signal-type.analog {
	background: var(--color-primary-light);
	color: var(--color-primary);
}

.signal-type.binary {
	background: var(--color-success-light);
	color: var(--color-success);
}

.signal-control {
	display: inline-flex;
	align-items: center;
	padding: 0.25rem 0.75rem;
	border-radius: 4px;
	font-size: 0.875rem;
	font-weight: 500;
}

.signal-control.controllable {
	background: var(--color-success-light);
	color: var(--color-success);
}

.signal-control.read-only {
	background: var(--color-primary-light);
	color: var(--color-primary);
}

.signal-archiving {
	display: inline-flex;
	align-items: center;
	padding: 0.25rem 0.75rem;
	border-radius: 4px;
	font-size: 0.875rem;
	font-weight: 500;
}

.signal-archiving.yes {
	background: var(--color-success-light);
	color: var(--color-success);
}

.signal-archiving.no {
	background: var(--color-primary-light);
	color: var(--color-primary);
}

.signal-unit {
	display: inline-flex;
	align-items: center;
	padding: 0.25rem 0.75rem;
	border-radius: 4px;
	font-size: 0.875rem;
	font-weight: 500;
	background: var(--color-primary-light);
	color: var(--color-primary);
}

.signal-unit .value {
	margin-left: 5px;
}

/* ----------------- Toggle Switch ----------------- */
.toggle-switch {
	position: relative;
	width: 50px;
	height: 24px;
}

.toggle-switch input {
	opacity: 0;
	width: 0;
	height: 0;
}

.toggle-switch label {
	position: absolute;
	cursor: pointer;
	top: 0;
	left: 0;
	right: 0;
	bottom: 0;
	background-color: #ccc;
	transition: .4s;
	border-radius: 34px;
}

.toggle-switch label:before {
	position: absolute;
	content: "";
	height: 16px;
	width: 16px;
	left: 4px;
	bottom: 4px;
	background-color: white;
	transition: .4s;
	border-radius: 50%;
}

.toggle-switch input:checked+label {
	background-color: var(--color-primary);
}

.toggle-switch input:checked+label:before {
	transform: translateX(26px);
}

/* ----------------- Misc ----------------- */
.settings-actions {
	display: flex;
	justify-content: flex-end;
	gap: 1rem;
	margin-top: 1rem;
}

.connection-placeholder {
	padding: 2rem;
	text-align: center;
	color: var(--color-text-secondary);
	background: var(--color-background-secondary);
	border-radius: 8px;
	margin: 1rem 0;
}

.danger-zone {
	margin-top: 2rem;
	padding: 1.5rem;
	border: 1px solid var(--color-danger);
	border-radius: 8px;
}

.loading-overlay {
	position: fixed;
	top: 0;
	left: 0;
	right: 0;
	bottom: 0;
	background: rgba(255, 255, 255, 0.8);
	display: flex;
	justify-content: center;
	align-items: center;
	z-index: 1100;
}

/* ----------------- Responsive Design ----------------- */
@media (max-width: 768px) {
	.settings-grid {
		grid-template-columns: 1fr;
	}

	.modal-content {
		width: 95%;
		padding: 1rem;
	}

	.device-card {
		flex-direction: column;
		align-items: flex-start;
		gap: 1rem;
	}

	.signal-details {
		flex-direction: column;
		gap: 0.5rem;
	}

	.signal-properties {
		flex-direction: column;
	}

	.modal-tabs {
		overflow-x: auto;
		padding-bottom: 0.5rem;
	}

	.settings-actions {
		flex-direction: column;
	}

	.action-button {
		width: 100%;
		justify-content: center;
	}

	.modal-footer {
		flex-direction: column-reverse;
	}

	.modal-footer .action-button {
		width: 100%;
		justify-content: center;
	}
}
</style>