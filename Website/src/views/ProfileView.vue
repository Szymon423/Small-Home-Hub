<template>
	<div class="profile-container">
		<h1 class="profile-title">
			<UserCircle class="profile-icon" />
			User Profile
		</h1>

		<div class="profile-grid">
			<!-- Personal Information Section -->
			<div class="profile-section">
				<h2 class="section-title">
					<User class="section-icon" />
					Personal Information
				</h2>
				<div class="profile-form">
					<div class="form-group">
						<label for="name">First Name</label>
						<div class="input-with-icon">
							<UserCircle class="input-icon" />
							<input type="text" id="name" v-model="profile.name" placeholder="Enter your first name">
						</div>
					</div>

					<div class="form-group">
						<label for="surname">Last Name</label>
						<div class="input-with-icon">
							<Users class="input-icon" />
							<input type="text" id="surname" v-model="profile.surname"
								placeholder="Enter your last name">
						</div>
					</div>
				</div>
			</div>

			<!-- Account Settings Section -->
			<div class="profile-section">
				<h2 class="section-title">
					<Settings class="section-icon" />
					Account Settings
				</h2>
				<div class="profile-form">
					<div class="form-group">
						<label for="login">Login</label>
						<div class="input-with-icon">
							<AtSign class="input-icon" />
							<input type="text" id="login" v-model="profile.login" placeholder="Enter your login">
						</div>
					</div>

					<div class="form-group">
						<label for="currentPassword">Current Password</label>
						<div class="input-with-icon">
							<Lock class="input-icon" />
							<input :type="showPassword ? 'text' : 'password'" id="currentPassword"
								v-model="profile.currentPassword" placeholder="Enter current password">
							<button class="password-toggle" @click="showPassword = !showPassword" type="button">
								<Eye v-if="!showPassword" class="password-icon" />
								<EyeOff v-else class="password-icon" />
							</button>
						</div>
					</div>

					<div class="form-group">
						<label for="newPassword">New Password</label>
						<div class="input-with-icon">
							<KeyRound class="input-icon" />
							<input :type="showNewPassword ? 'text' : 'password'" id="newPassword"
								v-model="profile.newPassword" placeholder="Enter new password">
							<button class="password-toggle" @click="showNewPassword = !showNewPassword" type="button">
								<Eye v-if="!showNewPassword" class="password-icon" />
								<EyeOff v-else class="password-icon" />
							</button>
						</div>
					</div>

					<div class="form-group">
						<label for="confirmPassword">Confirm New Password</label>
						<div class="input-with-icon">
							<KeyRound class="input-icon" />
							<input :type="showConfirmPassword ? 'text' : 'password'" id="confirmPassword"
								v-model="profile.confirmPassword" placeholder="Confirm new password">
							<button class="password-toggle" @click="showConfirmPassword = !showConfirmPassword"
								type="button">
								<Eye v-if="!showConfirmPassword" class="password-icon" />
								<EyeOff v-else class="password-icon" />
							</button>
						</div>
					</div>
				</div>
			</div>
		</div>

		<div class="profile-actions">
			<button class="action-button cancel" @click="resetProfile">
				<XCircle class="button-icon" />
				Cancel
			</button>
			<button class="action-button save" @click="saveProfile">
				<Save class="button-icon" />
				Save Changes
			</button>
		</div>
	</div>
</template>

<script setup lang="ts">
import { ref } from 'vue'
import {
	UserCircle,
	User,
	Users,
	Settings,
	AtSign,
	Lock,
	KeyRound,
	Eye,
	EyeOff,
	Save,
	XCircle
} from 'lucide-vue-next'

const profile = ref({
	name: '',
	surname: '',
	login: '',
	currentPassword: '',
	newPassword: '',
	confirmPassword: ''
})

const originalProfile = ref({})
const showPassword = ref(false)
const showNewPassword = ref(false)
const showConfirmPassword = ref(false)

// Simulation of fetching profile from API
const fetchProfile = async () => {
	// Here will be actual data fetching
	profile.value = {
		name: 'John',
		surname: 'Doe',
		login: 'john.doe',
		currentPassword: '',
		newPassword: '',
		confirmPassword: ''
	}
	originalProfile.value = { ...profile.value }
}

const saveProfile = async () => {
	// Here will be actual data saving with validation
	if (profile.value.newPassword !== profile.value.confirmPassword) {
		console.error('Passwords do not match')
		return
	}
	console.log('Saving profile:', profile.value)
	originalProfile.value = { ...profile.value }
}

const resetProfile = () => {
	profile.value = { ...originalProfile.value }
}

// Fetch profile when component is mounted
fetchProfile()
</script>

<style scoped>
.profile-container {
	padding: 20px;
	max-width: 1200px;
	margin: 0 auto;
}

.profile-title {
	display: flex;
	align-items: center;
	gap: 12px;
	color: #2c3e50;
	margin-bottom: 30px;
	font-size: 1.8rem;
}

.profile-icon {
	width: 28px;
	height: 28px;
	color: #3498db;
}

.profile-grid {
	display: grid;
	grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
	gap: 24px;
	margin-bottom: 30px;
}

.profile-section {
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

.profile-form {
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

.password-toggle {
	position: absolute;
	right: 12px;
	background: none;
	border: none;
	cursor: pointer;
	padding: 0;
	display: flex;
	align-items: center;
}

.password-icon {
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

input[type="password"] {
	padding-right: 40px;
}

input:focus {
	outline: none;
	border-color: #3498db;
	box-shadow: 0 0 0 3px rgba(52, 152, 219, 0.1);
}

input:hover {
	border-color: #3498db;
}

.profile-actions {
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
	.profile-grid {
		grid-template-columns: 1fr;
	}
}
</style>
