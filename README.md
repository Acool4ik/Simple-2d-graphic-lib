## Реализая простой графической бибилиотеки
### Принцип работы: 
отображение геометрических фигур из (R^2) на некоторое полотно (N*M). В качестве последнего были взяты растровые форматы изображений. В данный момент доступен только png, но можно без проблем добавить и другие. В качестве фигур реализован круг и квадрат а так же произвольные множества в виде набора пикселей.
### Пример:
![result](https://github.com/Acool4ik/Simple-2d-graphic-lib/blob/master/4K_result.png)
### Зависисмости:
```bash
sudo apt-get install libpng-dev && sudo apt-get install libpng
```
### Cобрать и запустить:
```bash
make && ./main && xdg-open ./result.png
```
### Примечание: 
в файлах Shape.(cpp|hpp) реализован паттерн "двойная диспетчеризация", который не очень прост для понимания и в первое время может быть не ясна причина его использования. Она заключается в том, чтобы исключить RTTI из проекта.
