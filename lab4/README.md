### Вимоги
* **ОС:** Linux (Ubuntu 20.04+)
* **Залізо:** Веб-камера, клавіатура, миша
* **ПЗ:** GCC, CMake, OpenCV 3.x або 4.x
* **Утиліти:** `wget` (для завантаження моделей нейромережі)
### 1. Підготовка скриптів
Всі скрипти повинні мати права на виконання. Якщо ні, виконайте команду:

```bash
chmod +x *.sh
```

Встановлення залежностей (потрібні права sudo):
```
./preinstall.sh
```

Збірка проекту:
```
./build.sh
```
Запуск:
```
./run.sh
```
Структура проекту
```
├── CMakeLists.txt
├── README.md
├── build.sh
├── preinstall.sh
├── run.sh
├── include
│   ├── CameraProvider.hpp
│   ├── Display.hpp
│   ├── FrameProcessor.hpp
│   ├── FaceDetector.hpp
│   └── KeyProcessor.hpp
└── src
    ├── CameraProvider.cpp
    ├── Display.cpp
    ├── FrameProcessor.cpp
    ├── FaceDetector.cpp
    ├── KeyProcessor.cpp
    └── main.cpp
```
### Керування:
| клавіша |                 дія |
| ------------- |:-------------:|
| 1      | Звичайний режим (без фільтрів)    |
| 2     | Інверсія кольорів (Negative) |
| 3  | Розмиття (Gaussian Blur)   | 
| 4     | Виділення границь (Canny Edge Detector)|
| 5     | Оператор Собеля (Sobel Operator) |
| 6  | Бінаризація (Threshold)  |
| 7      | Glitch ефект (зсув RGB каналів) |
| 8     | Картинка в картинці (PiP) |
| r/l  | Обертання зображення (праворуч / ліворуч) |
| +/-    | Зум (наближення / віддалення) |
| w/a/s/d   | Переміщення прицілу (хрестика) по екрану |
| F  | трекінг обличчя  |
| ESC  | Вихід з програми |

