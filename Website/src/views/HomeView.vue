<template>
    <div class="home-container">
        <LoadingOverlay 
            :is-visible="isLoading" 
            :message="loadingMessage"
        />

        <h1 class="home-title">
            <Home class="home-icon" />
            Home Automation Center
        </h1>

        <div class="groups-grid">
            <div v-for="group in groups" :key="group.GroupID" class="group-section">
                <h2 class="section-title">
                    <Home class="section-icon" />
                    {{ group.Name }}
                </h2>
                <p class="section-description">{{ group.Description }}</p>

                <div class="signals-list">
                    <div v-for="signal in getSignalsForGroup(group.GroupID)" 
                         :key="signal.ID" 
                         class="signal-card"
                         :class="{ 'alarm': hasAlarm(signal.ID) }">
                        <div class="signal-info">
                            <component 
                                :is="getSignalIcon(signal.DataType)"
                                class="signal-icon"
                            />
                            <div class="signal-details">
                                <span class="signal-name">{{ signal.Name }}</span>
                                <span class="signal-description">{{ signal.Description }}</span>
                            </div>
                        </div>
                        <div class="signal-value" :class="getValueClass(signal.ID)">
                            {{ formatSignalValue(signal) }}
                            {{ signal.Label.Unit }}
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</template>

<script setup lang="ts">
import { ref, onMounted } from 'vue'
import axios from 'axios'
import {
    Home,
    Activity,
    Thermometer,
    Droplets,
    Lightbulb,
    DoorClosed,
} from 'lucide-vue-next'
import LoadingOverlay from '../components/LoadingOverlay.vue'

interface Alarm {
    Enabled: boolean;
    Threshold: number;
}

interface Label {
    HighLabel: string;
    LowLabel: string;
    Unit: string;
}

interface Signal {
    AlarmHigh: Alarm;
    AlarmLow: Alarm;
    DataType: number;
    Description: string;
    GroupID: number;
    ID: number;
    IsArchived: boolean;
    Label: Label;
    Name: string;
}

interface Group {
    Description: string;
    GroupID: number;
    Name: string;
}

interface SignalValue {
    AlarmHigh: boolean;
    AlarmLow: boolean;
    Timestamp: number;
    Valid: boolean;
    Value: number | boolean;
}

interface SignalState {
    Current: SignalValue;
    Previous: SignalValue;
    ID: number;
}

const groups = ref<Group[]>([])
const signals = ref<Signal[]>([])
const signalValues = ref<SignalState[]>([])
const isLoading = ref(false)
const loadingMessage = ref('')

const getSignalsForGroup = (groupId: number) => {
    return signals.value.filter(signal => signal.GroupID === groupId)
}

const getSignalValue = (signalId: number) => {
    const value = signalValues.value.find(sv => sv.ID === signalId)
    return value?.Current
}

const hasAlarm = (signalId: number) => {
    const value = getSignalValue(signalId)
    return value ? (value.AlarmHigh || value.AlarmLow) : false
}

const getValueClass = (signalId: number) => {
    const value = getSignalValue(signalId)
    return {
        'value-invalid': value && !value.Valid,
        'value-alarm': value && (value.AlarmHigh || value.AlarmLow)
    }
}

const getSignalIcon = (dataType: number) => {
    switch (dataType) {
        case 1: return Thermometer // Dla sygnałów analogowych (temperatura, wilgotność)
        case 2: return Lightbulb   // Dla sygnałów binarnych (światła, przełączniki)
        default: return Activity
    }
}

const formatSignalValue = (signal: Signal) => {
    const value = getSignalValue(signal.ID)
    if (!value || !value.Valid) return 'N/A'

    if (signal.DataType === 2) { // Sygnał binarny
        return value.Value ? 'ON' : 'OFF'
    } else { // Sygnał analogowy
        return typeof value.Value === 'number' ? value.Value.toFixed(1) : value.Value
    }
}

const fetchStaticData = async () => {
    isLoading.value = true
    loadingMessage.value = 'Loading data...'

    try {
        const [groupsResponse, signalsResponse] = await Promise.all([
            axios.get('http://127.0.0.1:9999/api/signals/groups/get'),
            axios.get('http://127.0.0.1:9999/api/signals/definitions/get'),
        ])

        groups.value = groupsResponse.data
        signals.value = signalsResponse.data
    } catch (error) {
        console.error('Error fetching static data:', error)
    } finally {
        isLoading.value = false
    }
}

const fetchValues = async () => {
    try {
        const response = await axios.get('http://127.0.0.1:9999/api/signals/values/get')
        signalValues.value = response.data
    } catch (error) {
        console.error('Error fetching values:', error)
    }
}


onMounted(async () => {
    await fetchStaticData()
    await fetchValues()
    setInterval(fetchValues, 1000)
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
    border-left: 3px solid transparent;
}

.signal-card:hover {
    background: #f1f5f9;
    transform: translateX(4px);
}

.signal-card.alarm {
    border-left: 3px solid #ef4444;
    background: #fef2f2;
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
    min-width: 60px;
    text-align: center;
    transition: all 0.3s;
}

.value-invalid {
    background: #cbd5e1;
    color: #64748b;
    font-style: italic;
}

.value-alarm {
    background: #fee2e2;
    color: #ef4444;
    font-weight: 700;
    animation: pulse 2s infinite;
}

@keyframes pulse {
    0% {
        opacity: 1;
    }
    50% {
        opacity: 0.6;
    }
    100% {
        opacity: 1;
    }
}

/* Styl dla loadera */
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
    z-index: 1000;
}

.loading-message {
    background: white;
    padding: 20px;
    border-radius: 8px;
    box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
    display: flex;
    align-items: center;
    gap: 10px;
}

/* Responsywność */
@media (max-width: 768px) {
    .groups-grid {
        grid-template-columns: 1fr;
    }

    .home-container {
        padding: 10px;
    }

    .home-title {
        font-size: 1.5rem;
    }

    .signal-card {
        padding: 10px;
    }

    .signal-name {
        font-size: 0.85rem;
    }

    .signal-description {
        font-size: 0.75rem;
    }
}

/* Dodatkowe style dla różnych stanów sygnałów */
.signal-card[data-type="1"] .signal-icon {
    color: #3498db; /* Kolor dla sygnałów analogowych */
}

.signal-card[data-type="2"] .signal-icon {
    color: #f1c40f; /* Kolor dla sygnałów binarnych */
}

.signal-value.on {
    background: #dcfce7;
    color: #166534;
}

.signal-value.off {
    background: #f1f5f9;
    color: #64748b;
}

/* Hover efekty */
.signal-card:hover .signal-icon {
    transform: scale(1.1);
}

.signal-value:hover {
    transform: scale(1.05);
}

/* Transition efekty */
.signal-card,
.signal-icon,
.signal-value {
    transition: all 0.2s ease-in-out;
}
</style>
