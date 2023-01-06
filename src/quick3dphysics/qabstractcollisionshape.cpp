// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#include "qabstractcollisionshape_p.h"

#include <QtQml/QQmlListReference>

#include "qphysicsworld_p.h"

QT_BEGIN_NAMESPACE

/*!
    \qmltype CollisionShape
    \inherits Node
    \inqmlmodule QtQuick3DPhysics
    \since 6.4
    \brief Base type for collision shapes.

    This is the base type for all collision shapes.
*/

/*!
    \qmlproperty bool CollisionNode::enableDebugDraw
    This property enables drawing the shape's debug view.
*/

QAbstractCollisionShape::QAbstractCollisionShape(QQuick3DNode *parent) : QQuick3DNode(parent)
{
    connect(this, &QQuick3DNode::sceneScaleChanged, this,
            &QAbstractCollisionShape::handleScaleChange);
}

QAbstractCollisionShape::~QAbstractCollisionShape() = default;

bool QAbstractCollisionShape::enableDebugDraw() const
{
    return m_enableDebugDraw;
}

void QAbstractCollisionShape::setEnableDebugDraw(bool enableDebugDraw)
{
    if (m_enableDebugDraw == enableDebugDraw)
        return;

    if (auto world = QPhysicsWorld::getWorld(); world != nullptr && enableDebugDraw)
        world->setHasIndividualDebugDraw();

    m_enableDebugDraw = enableDebugDraw;
    emit enableDebugDrawChanged(m_enableDebugDraw);
}

void QAbstractCollisionShape::handleScaleChange()
{
    auto newScale = sceneScale();
    if (!qFuzzyCompare(newScale, m_prevScale)) {
        m_prevScale = newScale;
        m_scaleDirty = true;
        emit needsRebuild(this); // TODO: remove signal argument?
    }
}

QT_END_NAMESPACE
