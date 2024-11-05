<template>
	<div class="settings-container">
		<LoadingOverlay :is-visible="isLoading" :message="loadingMessage"/>
		<h1 class="settings-title">
			<Settings class="settings-icon" />
			Configuration
		</h1>

		<div class="settings-grid">
			<!-- Device Configuration Section -->
			<div class="settings-section">
				<h2 class="section-title">
					<Network class="section-icon" />
					Network Configuration
				</h2>
				<div class="settings-form">
					<div class="form-group">
						<label for="deviceIp">Device IP Address</label>
						<div class="input-with-icon">
							<Wifi class="input-icon" />
							<input type="text" id="deviceIp" v-model="settings.deviceIp"
								placeholder="e.g. 192.168.1.100">
						</div>
					</div>

					<div class="form-group">
						<label for="deviceSubnetMask">Subnet Mask</label>
						<div class="input-with-icon">
							<Network class="input-icon" />
							<input type="text" id="deviceSubnetMask" v-model="settings.deviceSubnetMask"
								placeholder="e.g. 255.255.255.0">
						</div>
					</div>
				</div>
			</div>

			<!-- Device Identity Section -->
			<div class="settings-section">
				<h2 class="section-title">
					<Server class="section-icon" />
					Device Identity
				</h2>
				<div class="settings-form">
					<div class="form-group">
						<label for="deviceName">Device Name</label>
						<div class="input-with-icon">
							<Tag class="input-icon" />
							<input type="text" id="deviceName" v-model="settings.deviceName"
								placeholder="e.g. Device-01">
						</div>
					</div>
				</div>
			</div>

			<!-- Server Configuration Section -->
			<div class="settings-section">
				<h2 class="section-title">
					<ServerCog class="section-icon" />
					Server Configuration
				</h2>
				<div class="settings-form">
					<div class="form-group">
						<label for="backendServerPort">Backend Server Port</label>
						<div class="input-with-icon">
							<Container class="input-icon" />
							<input type="number" id="backendServerPort" v-model="settings.backendServerPort"
								placeholder="e.g. 8080">
						</div>
					</div>

					<div class="form-group">
						<label for="communicationServerPort">Communication Server Port</label>
						<div class="input-with-icon">
							<Share2 class="input-icon" />
							<input type="number" id="communicationServerPort" v-model="settings.communicationServerPort"
								placeholder="e.g. 9090">
						</div>
					</div>
				</div>
			</div>

			<!-- Data Management Section -->
			<div class="settings-section">
				<h2 class="section-title">
					<Database class="section-icon" />
					Data Management
				</h2>
				<div class="settings-form">
					<div class="form-group">
						<label for="dataRetentionPeriod">Data Retention Period (days)</label>
						<div class="input-with-icon">
							<Clock class="input-icon" />
							<input type="number" id="dataRetentionPeriod" v-model="settings.dataRetentionPeriod"
								placeholder="e.g. 30">
						</div>
					</div>
				</div>
			</div>
		</div>

		<div class="settings-actions">
			<button class="action-button cancel" @click="resetSettings">
				<XCircle class="button-icon" />
				Cancel
			</button>
			<button class="action-button save" @click="saveSettings">
				<Save class="button-icon" />
				Save Changes
			</button>
		</div>
	</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import axios from 'axios'
import LoadingOverlay from '../components/loadingOverlay.vue'
import {
	Settings,
	Network,
	Wifi,
	Server,
	ServerCog,
	Container,
	Share2,
	Database,
	Clock,
	Tag,
	Save,
	XCircle
} from 'lucide-vue-next'

// For loading overlay
const isLoading = ref(false)
const loadingMessage = ref('')

const settings = ref({
	deviceIp: '',
	deviceSubnetMask: '',
	deviceName: '',
	dataRetentionPeriod: '',
	backendServerPort: '',
	communicationServerPort: ''
})

const originalSettings = ref({})

const fetchSettings = async () => {
    isLoading.value = true
    loadingMessage.value = 'Loading settings...'
    
    try {
        const response = await axios.get('http://localhost:9999/api/configuration/get')
        settings.value = response.data
        originalSettings.value = { ...response.data }
    } catch (error) {
        console.error('Błąd podczas pobierania ustawień:', error)
        throw error
    } finally {
        isLoading.value = false
    }
}

const saveSettings = async () => {
    const dataToUpdate = Object.entries(settings.value).reduce((acc, [key, value]) => {
        if (originalSettings.value[key] !== value) {
            acc[key] = value;
        }
        return acc;
    }, {});

    if (Object.keys(dataToUpdate).length === 0) {
        console.log('No changes to save');
        return;
    }

    isLoading.value = true
    loadingMessage.value = 'Saving changes...'

    try {
        const response = await axios.post('http://localhost:9999/api/configuration/update',
		    dataToUpdate, {
                headers: {
                    'Content-Type': 'application/json'
                }
            }
        );

        if (response.status === 200) {
            console.log('Settings saved successfully:', dataToUpdate);
            await fetchSettings();
        }
    } catch (error) {
        if (axios.isAxiosError(error)) {
            console.error('Błąd podczas zapisywania ustawień:', {
                message: error.message,
                status: error.response?.status,
                data: error.response?.data
            });
        } else {
            console.error('Nieoczekiwany błąd:', error);
        }
        throw error;
    } finally {
        isLoading.value = false
    }
};

const resetSettings = () => {
	settings.value = { ...originalSettings.value }
}

// Fetch settings when component is mounted
fetchSettings()
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
	margin-bottom: 30px;
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

input {
	width: 100%;
	padding: 10px 12px 10px 40px;
	border: 1px solid #e2e8f0;
	border-radius: 8px;
	font-size: 0.9rem;
	transition: all 0.3s;
}

input:focus {
	outline: none;
	border-color: #3498db;
	box-shadow: 0 0 0 3px rgba(52, 152, 219, 0.1);
}

input:hover {
	border-color: #3498db;
}

.settings-actions {
	display: flex;
	justify-content: flex-end;
	gap: 12px;
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

.button-icon {
	width: 18px;
	height: 18px;
}

.action-button.save {
	background-color: #3498db;
	color: white;
}

.action-button.save:hover {
	background-color: #2980b9;
}

.action-button.cancel {
	background-color: #e2e8f0;
	color: #64748b;
}

.action-button.cancel:hover {
	background-color: #cbd5e1;
}

@media (max-width: 768px) {
	.settings-grid {
		grid-template-columns: 1fr;
	}
}
</style>