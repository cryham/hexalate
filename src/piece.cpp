/***********************************************************************
 *
 * Copyright (C) 2009 Graeme Gott <graeme@gottcode.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***********************************************************************/

#include "piece.h"

#include "puzzle.h"

#include <QGraphicsItemAnimation>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QPointF>
#include <QPolygonF>
#include <QRadialGradient>
#include <QTimeLine>
#include <QTimer>

/*****************************************************************************/

Piece::Piece(Puzzle* puzzle)
: QGraphicsEllipseItem(0, 0, 100, 100),
  m_puzzle(puzzle),
  m_gradient(50, 50, 90),
  m_rotations(0),
  m_swap_piece(0) {
	for (int i = 0; i < 6; ++i) {
		m_colors.append(i);
		m_connectors.append(-1);
	}
	setFlag(ItemIsMovable, true);
	setCacheMode(DeviceCoordinateCache);
	setHighlight(false);

	for (int i = 0; i < 24; ++i) {
		m_gradient.setColorAt(static_cast<float>(i) / 24.0f, Qt::black);
	}

	m_rotate_timer = new QTimer(this);
	connect(m_rotate_timer, SIGNAL(timeout()), this, SLOT(rotateConnectors()));
	m_rotate_timer->setInterval(40);

	m_puzzle->addItem(this);

	// Create middle of piece
	QGraphicsEllipseItem* overlay = new QGraphicsEllipseItem(0, 0, 100, 100, this);
	overlay->setPen(Qt::NoPen);
	QRadialGradient gradient(50, 50, 50);
	gradient.setColorAt(0.05, Qt::black);
	gradient.setColorAt(0.15, Qt::transparent);
	overlay->setBrush(gradient);
}

/*****************************************************************************/

bool Piece::setConnector(int offset, int value) {
	if (m_colors.contains(value)) {
		m_connectors[offset] = value;
		m_colors.removeOne(value);

		static QColor connector_colors[6] = {
			QColor(255, 0, 0),
			QColor(255, 255, 0),
			QColor(0, 255, 0),
			QColor(0, 255, 255),
			QColor(0, 0, 255),
			QColor(255, 0, 255)
		};
		m_gradient.setColorAt(static_cast<float>(6 - offset) / 6.0f, connector_colors[value]);
		if (offset == 0) {
			m_gradient.setColorAt(0, connector_colors[value]);
		}
		setBrush(m_gradient);

		return true;
	} else {
		return false;
	}
}

/*****************************************************************************/

void Piece::setHighlight(bool highlight) {
	setCacheMode(NoCache);
	setPen(QPen(highlight ? Qt::white : Qt::darkGray, 2));
	setZValue(highlight ? 2 : 1);
	setCacheMode(DeviceCoordinateCache);
}

/*****************************************************************************/

void Piece::setPosition(const QPointF& position) {
	m_position = position;
	setPos(m_position);
}

/*****************************************************************************/

void Piece::spin() {
	int rotations = rand() % 6;
	int angle = 90;
	for (int i = 0; i < rotations; ++i) {
		m_connectors.move(5, 0);
		angle -= 60;
	}
	if (angle < 0) {
		angle += 360;
	}
	m_gradient.setAngle(angle);
	setBrush(m_gradient);
}

/*****************************************************************************/

bool Piece::fromString(const QString& string) {
	QStringList values = string.split(",", QString::SkipEmptyParts);
	if (values.count() != 6) {
		return false;
	}

	for (int i = 0; i < 6; ++i) {
		if (!setConnector(i, values[i].toInt())) {
			return false;
		}
	}
	return true;
}

/*****************************************************************************/

QString Piece::toString() const {
	QStringList values;
	for (int i = 0; i < 6; ++i) {
		values.append(QString::number(m_connectors[i]));
	}
	return values.join(",");
}

/*****************************************************************************/

void Piece::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event) {
	rotate();
	QGraphicsEllipseItem::mouseDoubleClickEvent(event);
}

/*****************************************************************************/

void Piece::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
	// Find piece containing center
	Piece* swap_piece = 0;
	QList<QGraphicsItem*> items = scene()->items(sceneBoundingRect().center());
	foreach (QGraphicsItem* item, items) {
		Piece* piece = qgraphicsitem_cast<Piece*>(item);
		if (piece && piece != this) {
			swap_piece = piece;
			break;
		}
	}

	// Highlight swap piece
	if (swap_piece) {
		if (swap_piece != m_swap_piece) {
			if (m_swap_piece) {
				m_swap_piece->setHighlight(false);
			}
			m_swap_piece = swap_piece;
			m_swap_piece->setHighlight(true);
		}
	} else {
		if (m_swap_piece) {
			m_swap_piece->setHighlight(false);
			m_swap_piece = 0;
		}
	}

	QGraphicsEllipseItem::mouseMoveEvent(event);
}

/*****************************************************************************/

void Piece::mousePressEvent(QGraphicsSceneMouseEvent* event) {
	if (event->button() == Qt::LeftButton) {
		setZValue(3);
		m_swap_piece = 0;
	} else if (event->button() == Qt::RightButton) {
		rotate();
	}
	QGraphicsEllipseItem::mousePressEvent(event);
}

/*****************************************************************************/

void Piece::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {
	if (m_swap_piece) {
		m_puzzle->swapPieces(this, m_swap_piece);
		qSwap(m_position, m_swap_piece->m_position);
		m_swap_piece->setHighlight(false);
		m_swap_piece->setZValue(2);
		m_swap_piece->moveTo(m_swap_piece->m_position);
		m_swap_piece = 0;
	}
	moveTo(m_position);
	QGraphicsEllipseItem::mouseReleaseEvent(event);
}

/*****************************************************************************/

void Piece::rotateConnectors() {
	float angle = m_gradient.angle();
	angle -= 20;
	if (angle < 0) {
		angle += 360;
	}
	m_gradient.setAngle(angle);
	setBrush(m_gradient);

	m_rotations--;
	if (m_rotations == 0) {
		m_rotate_timer->stop();
		actionFinished();
	}
}

/*****************************************************************************/

void Piece::actionFinished() {
	setZValue(1);
	if (!m_puzzle->isDone()) {
		setFlag(ItemIsMovable, true);
	}
}

/*****************************************************************************/

void Piece::moveTo(const QPointF& new_pos) {
	setFlag(ItemIsMovable, false);

	QTimeLine* timeline = new QTimeLine(200, this);

	QGraphicsItemAnimation* animation = new QGraphicsItemAnimation(this);
	animation->setItem(this);
	animation->setTimeLine(timeline);
	animation->setPosAt(0, pos());
	animation->setPosAt(1, new_pos);

	connect(timeline, SIGNAL(finished()), this, SLOT(actionFinished()));
	connect(timeline, SIGNAL(finished()), animation, SLOT(deleteLater()));
	connect(timeline, SIGNAL(finished()), timeline, SLOT(deleteLater()));
	timeline->start();
}

/*****************************************************************************/

void Piece::rotate() {
	m_connectors.move(5, 0);
	m_rotations += 3;
	if (!m_rotate_timer->isActive()) {
		m_rotate_timer->start();
		setFlag(ItemIsMovable, false);
	}
}

/*****************************************************************************/
