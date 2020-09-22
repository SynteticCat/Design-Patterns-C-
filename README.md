# 23 Паттерна проектирования на С++ ♻

Ниже описаны все паттерны проектирования из книги "Банды четырех" и к каждому приложена реализация на языке программирования С++

## Порождающие паттерны ⬇

1. Фабричный метод  

Описание:  
Порождающий шаблон проектирования, предоставляющий дочерним  классам интерфейс для создания экземпляров некоторого класса. В момент создания наследники могут определить, какой экземпляр класса создавать. Иными словами шаблон делегирует создание объектов наследникам родительского класса. Это позволит использовать в коде программы не специфические классы, а манипулировать абстрактными объектами на более высоком уровне.
  
Структура:  
- Product - продукт
  - интерфейс объектов, создаваемых абстрактным методом
- ConcreteProduct - конкретный продукт
  - реализует интерфейс Product
- Creator - создатель
  - реализует фабричный метод, который возвращает объект типа Product
  - может вызывать фабричный метод так, что он создает и возвращает объект Product
- ConcreteCreator - конкретный создатель
  - переопределяет фабричный метод так, что он создает и возвращает объект ConcreteProduct
  
![alt](https://upload.wikimedia.org/wikipedia/ru/f/f0/FactoryMethodPattern.png)

2. Абстрактная фабрика  
3. Строитель  
4. Прототип  
5. Одиночка  

## Структурные паттерны ⬇

## Поведенческие паттерны ⬇

