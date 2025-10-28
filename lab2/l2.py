import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider, Button, CheckButtons
from scipy import signal

INIT_AMPLITUDE = 1.0
INIT_FREQUENCY = 1.0 
INIT_PHASE = 0.0     

INIT_NOISE_MEAN = 0.0
INIT_NOISE_COVARIANCE = 0.2 

INIT_CUTOFF_FREQUENCY = 1.5
ORDER = 5                    

SAMPLING_RATE = 1000  
TIME_END = 10        
T = np.linspace(0, TIME_END, TIME_END * SAMPLING_RATE, endpoint=False)
N = len(T)

global_noise_sample = np.random.normal(INIT_NOISE_MEAN, np.sqrt(INIT_NOISE_COVARIANCE), N)
last_noise_params = {'mean': INIT_NOISE_MEAN, 'covariance': INIT_NOISE_COVARIANCE}


def harmonic_with_noise(amplitude, frequency, phase, noise_mean, noise_covariance, show_noise):
    """
    Реалізує функцію y(t) = A*sin(ωt + φ) + noise, з логікою збереження шуму.
    """
    global global_noise_sample
    global last_noise_params

    omega = 2 * np.pi * frequency
    clean_harmonic = amplitude * np.sin(omega*T + phase)

    noise_covariance_safe = max(0, noise_covariance)
    
    params_changed = not(np.isclose(noise_mean, last_noise_params['mean'], atol=1e-5) and np.isclose(noise_covariance_safe, last_noise_params['covariance'], atol=1e-5))
                     
    if params_changed:
        noise_std = np.sqrt(noise_covariance_safe)
        global_noise_sample = np.random.normal(noise_mean, noise_std, N)
        
        last_noise_params['mean'] = noise_mean
        last_noise_params['covariance'] = noise_covariance_safe
    

    if show_noise:
        noisy_harmonic = clean_harmonic + global_noise_sample
    else:
        noisy_harmonic = clean_harmonic
    return clean_harmonic, noisy_harmonic

def filter_signal(signal_data, cutoff_frequency, sampling_rate, order):
    quist = 0.5 * sampling_rate
    
    if cutoff_frequency <= 0 or cutoff_frequency >= quist:
        return signal_data
        
    normalized_cutoff = cutoff_frequency / quist
    b, a = signal.butter(order, normalized_cutoff, btype='low', analog=False)
    filtered_signal = signal.filtfilt(b, a, signal_data)
    
    return filtered_signal
 
 
def update_plot(val):
    
    amplitude = slider_amplitude.val
    frequency = slider_frequency.val
    phase = slider_phase.val
    noise_mean = slider_noise_mean.val
    noise_covariance = slider_noise_covariance.val
    cutoff_frequency = slider_cutoff_frequency.val

    statuses = checkbox_options.get_status()
    show_noise_toggle = statuses[0]
    show_filtered_toggle = statuses[1]

    clean_harmonic, noisy_harmonic = harmonic_with_noise(
        amplitude, frequency, phase, noise_mean, noise_covariance, show_noise_toggle
    )

    filtered_harmonic = filter_signal(
        clean_harmonic + global_noise_sample, cutoff_frequency, SAMPLING_RATE, ORDER
    )

    line_noisy.set_ydata(noisy_harmonic)
    
    line_filtered.set_ydata(filtered_harmonic)
    line_filtered.set_visible(show_filtered_toggle)
    
    line_clean.set_ydata(clean_harmonic)
    line_clean.set_visible(show_filtered_toggle) 
    
  
    line_noisy.set_label('Зашумлений Сигнал' if show_noise_toggle else 'Чистий Сигнал')
    ax.legend(loc='upper right') 

    data_to_check = [noisy_harmonic]
    if show_filtered_toggle:
        data_to_check.extend([filtered_harmonic, clean_harmonic])
        
    if data_to_check:
        max_abs_val = max(np.max(np.abs(d)) for d in data_to_check)
        ax.set_ylim(-max_abs_val - 0.5, max_abs_val + 0.5)

    fig.canvas.draw_idle()

def reset_plot(event):
    global global_noise_sample
    global last_noise_params
    
    slider_amplitude.reset()
    slider_frequency.reset()
    slider_phase.reset()
    slider_noise_mean.reset()
    slider_noise_covariance.reset()
    slider_cutoff_frequency.reset() 

    
    global_noise_sample = np.random.normal(INIT_NOISE_MEAN, np.sqrt(INIT_NOISE_COVARIANCE), N)
    last_noise_params['mean'] = INIT_NOISE_MEAN
    last_noise_params['covariance'] = INIT_NOISE_COVARIANCE

    current_statuses = checkbox_options.get_status()
    if not current_statuses[0]: checkbox_options.set_active(0)
    if current_statuses[1]: checkbox_options.set_active(1)
    
    update_plot(None)
    

fig, ax = plt.subplots(figsize=(10, 7))
plt.subplots_adjust(left=0.1, bottom=0.45, right=0.95, top=0.9)

clean_init, noisy_init = harmonic_with_noise(
    INIT_AMPLITUDE, INIT_FREQUENCY, INIT_PHASE, INIT_NOISE_MEAN, INIT_NOISE_COVARIANCE, True
)
filtered_init = filter_signal(clean_init + global_noise_sample, INIT_CUTOFF_FREQUENCY, SAMPLING_RATE, ORDER)

line_noisy, = ax.plot(T, noisy_init, color='orange', linewidth=2, label='Зашумлений Сигнал')
line_filtered, = ax.plot(T, filtered_init, color='blue', linewidth=3, label='Відфільтрований Сигнал', visible=False)
line_clean, = ax.plot(T, clean_init, color='purple', linestyle='--', linewidth=1.5, label='Чиста Гармоніка', visible=False)

ax.set_title(f'Інтерактивна гармоніка з шумом({ORDER}-го порядку)')
ax.set_xlabel('Час')
ax.set_ylabel('Амплітуда')
ax.grid(True)
ax.legend(loc='upper right')
max_init = np.max(np.abs(noisy_init))
ax.set_ylim(-max_init - 0.5, max_init + 0.5)

instructions = """
Чиста Гармоніка - фіолетова, пунктирна лінія - оригінальний сигнал y(t) = A * sin(ω*t + φ)
Основний Сигнал - помаранчева лінія - гармоніка з доданим шумом або без нього
Результат Фільтрації - синя лінія - згладжування
Amplitude, Frequency, Phase: Змінюють чисту гармоніку.
Noise Mean, Noise Covariance: Змінюють параметри шуму (шум регенерується тільки при зміні цих двох слайдерів).
Cutoff Frequency (Гц): Чим нижче значення, тим більше шуму фільтр видаляє.
Show Noise: Перемикає додавання шуму.
Show Filtered/Clean: Перемикає видимість фіолетової та синьої ліній.
Reset: Скидає всі налаштування до початкових.
"""
fig.text(0.65, 0.35
         ,instructions,
         verticalalignment='top', 
         fontsize=5,
         color='black',
         bbox=dict(boxstyle="round,pad=0.3", fc="lightgray", alpha=0.3), # Рамка
         wrap=False)

axcolor = 'lightgoldenrodyellow'
slider_width = 0.5

ax_amp = fig.add_axes([0.1, 0.35, slider_width, 0.03], facecolor=axcolor)
ax_freq = fig.add_axes([0.1, 0.3, slider_width, 0.03], facecolor=axcolor)
ax_phase = fig.add_axes([0.1, 0.25, slider_width, 0.03], facecolor=axcolor)

ax_n_mean = fig.add_axes([0.1, 0.2, slider_width, 0.03], facecolor=axcolor)
ax_n_cov = fig.add_axes([0.1, 0.15, slider_width, 0.03], facecolor=axcolor)


ax_cutoff = fig.add_axes([0.1, 0.1, slider_width, 0.03], facecolor=axcolor)

slider_amplitude = Slider(ax_amp, 'Amplitude', 0.1, 2.0, valinit=INIT_AMPLITUDE)
slider_frequency = Slider(ax_freq, 'Frequency', 0.1, 5.0, valinit=INIT_FREQUENCY)
slider_phase = Slider(ax_phase, 'Phase', -np.pi, np.pi, valinit=INIT_PHASE)
slider_noise_mean = Slider(ax_n_mean, 'Noise Mean', -0.5, 0.5, valinit=INIT_NOISE_MEAN)
slider_noise_covariance = Slider(ax_n_cov, 'Noise Covariance', 0.0, 0.5, valinit=INIT_NOISE_COVARIANCE)
slider_cutoff_frequency = Slider(ax_cutoff, 'Cutoff Frequency', 0.1, 5.0, valinit=INIT_CUTOFF_FREQUENCY)

slider_amplitude.on_changed(update_plot)
slider_frequency.on_changed(update_plot)
slider_phase.on_changed(update_plot)
slider_noise_mean.on_changed(update_plot)
slider_noise_covariance.on_changed(update_plot)
slider_cutoff_frequency.on_changed(update_plot)

ax_reset = fig.add_axes([0.1, 0.025, 0.1, 0.04])
button_reset = Button(ax_reset, 'Reset', color=axcolor, hovercolor='0.975')
button_reset.on_clicked(reset_plot)

ax_check = fig.add_axes([0.65, 0.025, 0.3, 0.04]) 
checkbox_options = CheckButtons(
    ax=ax_check,
    labels=['Show Noise', 'Show Filtered/Clean'],
    actives=[True, False] 
)
checkbox_options.on_clicked(update_plot)

plt.show()
