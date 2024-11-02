<template>
	<div class="app-container">
		<!-- Top Bar -->
		<div class="top-bar">
			<div class="menu-toggle">
				<button class="toggle-button" @click="toggleNav">
					<PanelLeftClose v-if="isNavCollapsed" />
					<PanelLeftOpen v-else />
				</button>
			</div>
			<h1 class="device-name">Device Name</h1>
			<div class="user-section">
				<div v-if="isLoggedIn" class="user-info">
					<User class="user-icon" />
					<span class="user-initials">JK</span>
				</div>
				<button class="auth-button" @click="toggleAuth">
					{{ isLoggedIn ? 'Logout' : 'Login' }}
					<LogOut v-if="isLoggedIn" class="button-icon" />
					<LogIn v-else class="button-icon" />
				</button>
			</div>
		</div>

		<div class="main-content">
			<!-- Left Navigation -->
			<nav class="side-nav" :class="{ 'collapsed': isNavCollapsed }">
				<RouterLink to="/" class="nav-link">
					<Home class="nav-icon" />
					<span class="nav-text">Home</span>
				</RouterLink>
				<RouterLink to="/configuration" class="nav-link">
					<LayoutDashboard class="nav-icon" />
					<span class="nav-text">Configuration</span>
				</RouterLink>
				<RouterLink to="/profile" class="nav-link">
					<User class="nav-icon" />
					<span class="nav-text">Profile</span>
				</RouterLink>
				<RouterLink to="/settings" class="nav-link">
					<Settings class="nav-icon" />
					<span class="nav-text">Settings</span>
				</RouterLink>
			</nav>

			<!-- Main Content Area -->
			<main class="content">
				<RouterView />
			</main>
		</div>

		<!-- Bottom Bar -->
		<div class="bottom-bar">
			<div class="time">{{ formattedDateTime }}</div>
		</div>
	</div>
</template>

<script setup lang="ts">
import { ref, onMounted, onUnmounted, computed } from 'vue'
import { RouterLink, RouterView } from 'vue-router'
import {
	PanelLeftOpen,
	PanelLeftClose,
	Home,
	LayoutDashboard,
	Settings,
	User,
	LogIn,
	LogOut
} from 'lucide-vue-next'

const currentTime = ref('')
const isLoggedIn = ref(false)
const isNavCollapsed = ref(false)
const userName = ref('Jan Kowalski')
const userInitials = computed(() => {
  return userName.value
    .split(' ')
    .map(name => name[0])
    .join('')
})
const toggleNav = () => {
	isNavCollapsed.value = !isNavCollapsed.value
}

const formattedDateTime = computed(() => {
	const now = new Date()
	const date = now.toISOString().split('T')[0]
	const time = now.toLocaleTimeString('en-GB', {
		hour: '2-digit',
		minute: '2-digit'
	})
	return `${date} ${time}`
})

const updateTime = () => {
	currentTime.value = new Date().toISOString()
}

const toggleAuth = () => {
	isLoggedIn.value = !isLoggedIn.value
}

let timeInterval: number

onMounted(() => {
	updateTime()
	timeInterval = setInterval(updateTime, 1000)
})

onUnmounted(() => {
	clearInterval(timeInterval)
})
</script>

<style>
@import url('https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600&display=swap');

* {
	font-family: 'Poppins', sans-serif;
}
</style>

<style scoped>
.app-container {
	display: flex;
	flex-direction: column;
	min-height: 100vh;
	background-color: #f5f5f5;
}

.top-bar {
	background-color: #ffffff;
	height: 60px;
	display: flex;
	justify-content: space-between;
	align-items: center;
	padding: 0 20px;
	box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
	position: relative;
	z-index: 2;
}

.menu-toggle {
	display: flex;
	align-items: center;
}

.toggle-button {
	background: none;
	border: none;
	cursor: pointer;
	padding: 8px;
	color: #2c3e50;
	display: flex;
	align-items: center;
	justify-content: center;
	border-radius: 8px;
	transition: background-color 0.3s;
}

.toggle-button:hover {
	background-color: #f0f0f0;
}

.device-name {
	color: #2c3e50;
	font-size: 1.2rem;
	font-weight: 500;
}

.user-section {
  display: flex;
  align-items: center;
  gap: 16px;
}

.user-info {
  display: flex;
  align-items: center;
  gap: 8px;
  color: #2c3e50;
}

.user-icon {
  width: 20px;
  height: 20px;
}

.user-initials {
  font-weight: 500;
  font-size: 0.9rem;
}

.auth-button {
	background-color: #3498db;
	color: white;
	border: none;
	padding: 8px 16px;
	border-radius: 8px;
	cursor: pointer;
	transition: background-color 0.3s;
	display: flex;
	align-items: center;
	gap: 8px;
	font-weight: 500;
	margin-left: 8px;
}

.button-icon {
	width: 18px;
	height: 18px;
}

.auth-button:hover {
	background-color: #2980b9;
}

.main-content {
	display: flex;
	flex: 1;
	position: relative;
}

.side-nav {
	width: 240px;
	background-color: #ffffff;
	padding: 20px 0;
	box-shadow: 2px 0 4px rgba(0, 0, 0, 0.1);
	transition: width 0.3s ease;
	position: relative;
	z-index: 1;
}

.side-nav.collapsed {
	width: 72px;
}

.nav-link {
	display: flex;
	align-items: center;
	padding: 12px 20px;
	color: #2c3e50;
	text-decoration: none;
	transition: all 0.3s;
	border-radius: 8px;
	margin: 4px 8px;
}

.nav-icon {
	width: 20px;
	height: 20px;
	min-width: 20px;
}

.nav-text {
	margin-left: 12px;
	font-weight: 500;
}

.collapsed .nav-text {
	display: none;
}

.nav-link:hover {
	background-color: #f0f0f0;
}

.nav-link.router-link-active {
	background-color: #e3f2fd;
	color: #3498db;
}

.content {
	flex: 1;
	padding: 20px;
	background-color: #ffffff;
	margin: 20px;
	border-radius: 8px;
	box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
}

.bottom-bar {
	background-color: #ffffff;
	height: 40px;
	display: flex;
	justify-content: flex-end;
	align-items: center;
	padding: 0 20px;
	box-shadow: 0 -2px 4px rgba(0, 0, 0, 0.1);
	position: relative;
	z-index: 2;
}

.time {
	color: #2c3e50;
	font-size: 0.9rem;
	font-weight: 500;
}

@media (max-width: 768px) {
	.user-initials {
	display: none;
	}
}
</style>
