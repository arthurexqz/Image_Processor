# Image Processor

Консольная программа на C++ для применения фильтров к изображениям BMP.

## Сборка

Нужны CMake и C++-компилятор с поддержкой C++17.

```bash
cmake -S . -B build
cmake --build build
```

## Использование

```text
./build/image_processor <входной.bmp> <выходной.bmp> [фильтры]
```

Пример:

```bash
./build/image_processor test_script/data/lenna.bmp output/lenna_gray.bmp -gs
```

Фильтры применяются по порядку. Если фильтры не указаны, изображение сохраняется без изменений.

## Фильтры

### Обрезка (`-crop width height`)

Обрезает изображение от верхнего левого угла до заданного размера.

```bash
./build/image_processor test_script/data/lenna.bmp output/lenna_crop.bmp -crop 800 600
```

### Оттенки серого (`-gs`)

Переводит изображение в оттенки серого.

```bash
./build/image_processor test_script/data/lenna.bmp output/lenna_gray.bmp -gs
```

### Негатив (`-neg`)

Преобразует изображение в негатив.

```bash
./build/image_processor test_script/data/lenna.bmp output/lenna_negative.bmp -neg
```

### Повышение резкости (`-sharp`)

Применяет фильтр повышения резкости.

```bash
./build/image_processor test_script/data/lenna.bmp output/lenna_sharp.bmp -sharp
```

### Выделение границ (`-edge threshold`)

Выделяет границы с указанным порогом.

```bash
./build/image_processor test_script/data/lenna.bmp output/lenna_edges.bmp -edge 0.2
```

### Гауссово размытие (`-blur sigma`)

Размывает изображение; параметр задаёт сигму.

```bash
./build/image_processor test_script/data/lenna.bmp output/lenna_blur.bmp -blur 1.0
```

### Кристаллизация (`-crystal size`)

Применяет эффект кристаллизации с заданным размером блоков.

```bash
./build/image_processor test_script/data/lenna.bmp output/lenna_crystal.bmp -crystal 10
```

Несколько фильтров можно объединить в одной команде:

```bash
./build/image_processor input.bmp output.bmp -crop 800 600 -gs -blur 0.5
```

## Формат изображений

Используйте несжатые 24-битные BMP-файлы. Результат также сохраняется в формате BMP.
