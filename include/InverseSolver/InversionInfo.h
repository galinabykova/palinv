#pragma once

/**
 * Контекст. Передаётся в переопределяемые функции. 
 * Для передачи дополнительной информации в прямую задачу нужно отнаследователься от этого класса
 */
class InversionInfo
{
public:
	virtual ~InversionInfo() = default;

	/**
	 * Возвращает максимальное количество итераций для инверсии
	 */
	virtual int GetIterationNumber() const;
};
