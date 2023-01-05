// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

#ifndef DYNAMICSWORLD_H
#define DYNAMICSWORLD_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtQuick3DPhysics/qtquick3dphysicsglobal.h>

#include <QtCore/QLoggingCategory>
#include <QtCore/QObject>
#include <QtCore/QTimerEvent>
#include <QtCore/QElapsedTimer>
#include <QtGui/QVector3D>
#include <QtQml/qqml.h>
#include <QBasicTimer>

#include <QtQuick3D/private/qquick3dviewport_p.h>

namespace physx {
class PxMaterial;
class PxPhysics;
class PxShape;
class PxRigidDynamic;
class PxRigidActor;
class PxRigidStatic;
class PxCooking;
class PxControllerManager;
}

QT_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(lcQuick3dPhysics);

class QAbstractCollisionNode;
class QAbstractCollisionShape;
class QAbstractRigidBody;
class QAbstractPhysXNode;
class QQuick3DModel;
class QQuick3DDefaultMaterial;
struct PhysXWorld;

class Q_QUICK3DPHYSICS_EXPORT QDynamicsWorld : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool forceDebugDraw READ forceDebugDraw WRITE setForceDebugDraw NOTIFY
                       forceDebugDrawChanged)
    Q_PROPERTY(bool enableCCD READ enableCCD WRITE setEnableCCD NOTIFY enableCCDChanged)
    Q_PROPERTY(float typicalLength READ typicalLength WRITE setTypicalLength NOTIFY
                       typicalLengthChanged)
    Q_PROPERTY(
            float typicalSpeed READ typicalSpeed WRITE setTypicalSpeed NOTIFY typicalSpeedChanged)
    Q_PROPERTY(float defaultDensity READ defaultDensity WRITE setDefaultDensity NOTIFY
                       defaultDensityChanged)
    Q_PROPERTY(
            QQuick3DViewport *sceneView READ sceneView WRITE setSceneView NOTIFY sceneViewChanged)
    Q_PROPERTY(float minimumTimestep READ minimumTimestep WRITE setMinimumTimestep NOTIFY
                       minimumTimestepChanged REVISION(6, 5))
    Q_PROPERTY(float maximumTimestep READ maximumTimestep WRITE setMaximumTimestep NOTIFY
                       maximumTimestepChanged REVISION(6, 5))

    QML_NAMED_ELEMENT(DynamicsWorld)

public:
    explicit QDynamicsWorld(QObject *parent = nullptr);
    ~QDynamicsWorld();

    void classBegin() override;
    void componentComplete() override;

    QVector3D gravity() const;

    bool running() const;
    bool forceDebugDraw() const;
    bool enableCCD() const;
    float typicalLength() const;
    float typicalSpeed() const;
    float defaultDensity() const;
    Q_REVISION(6, 5) float minimumTimestep() const;
    Q_REVISION(6, 5) float maximumTimestep() const;

    void registerOverlap(physx::PxRigidActor *triggerActor, physx::PxRigidActor *otherActor);
    void deregisterOverlap(physx::PxRigidActor *triggerActor, physx::PxRigidActor *otherActor);

    bool hasSendContactReports(QAbstractCollisionNode *object) const;
    bool hasReceiveContactReports(QAbstractCollisionNode *object) const;

    static QDynamicsWorld *getWorld()
    {
        return self; // TODO: proper mechanism for finding "my" world.
    }

    void registerNode(QAbstractCollisionNode *collisionNode);
    void deregisterNode(QAbstractCollisionNode *collisionNode);

    QQuick3DViewport *sceneView() const;

    void setHasIndividualDebugDraw();

    physx::PxControllerManager *controllerManager();

public slots:
    void setGravity(QVector3D gravity);
    void setRunning(bool running);
    void setForceDebugDraw(bool forceDebugDraw);
    void setEnableCCD(bool enableCCD);
    void setTypicalLength(float typicalLength);
    void setTypicalSpeed(float typicalSpeed);
    void setDefaultDensity(float defaultDensity);
    void setSceneView(QQuick3DViewport *sceneView);
    Q_REVISION(6, 5) void setMinimumTimestep(float minTimestep);
    Q_REVISION(6, 5) void setMaximumTimestep(float maxTimestep);

signals:
    void gravityChanged(QVector3D gravity);
    void runningChanged(bool running);
    void enableCCDChanged(bool enableCCD);
    void forceDebugDrawChanged(bool forceDebugDraw);
    void typicalLengthChanged(float typicalLength);
    void typicalSpeedChanged(float typicalSpeed);
    void defaultDensityChanged(float defaultDensity);
    void sceneViewChanged(QQuick3DViewport *sceneView);
    Q_REVISION(6, 5) void minimumTimestepChanged(float minimumTimestep);
    Q_REVISION(6, 5) void maximumTimestepChanged(float maxTimestep);
    void simulateFrame(float minTimestep, float maxTimestep);
    Q_REVISION(6, 5) void frameDone(float timestep);

private:
    void frameFinished(float deltaTime);
    void initPhysics();
    void cleanupRemovedNodes();
    void updateDebugDraw();
    void disableDebugDraw();

    struct DebugModelHolder
    {
        QQuick3DModel *model = nullptr;
        QVector3D data;

        const QVector3D &halfExtents() const { return data; }
        void setHalfExtents(const QVector3D &halfExtents) { data = halfExtents; }

        float radius() const { return data.x(); }
        void setRadius(float radius) { data.setX(radius); }

        float heightScale() const { return data.x(); }
        void setHeightScale(float heightScale) { data.setX(heightScale); }

        float halfHeight() const { return data.y(); }
        void setHalfHeight(float halfHeight) { data.setY(halfHeight); }

        float rowScale() const { return data.y(); }
        void setRowScale(float rowScale) { data.setY(rowScale); }

        float columnScale() const { return data.z(); }
        void setColumnScale(float columnScale) { data.setZ(columnScale); }
    };

    QList<QAbstractPhysXNode *> m_physXBodies;
    QList<QAbstractCollisionNode *> m_newCollisionNodes;
    QMap<QAbstractCollisionShape *, DebugModelHolder> m_collisionShapeDebugModels;
    QSet<QAbstractCollisionNode *> m_removedCollisionNodes;

    QVector3D m_gravity = QVector3D(0.f, -981.f, 0.f);
    float m_typicalLength = 100.f; // 100 cm
    float m_typicalSpeed = 1000.f; // 1000 cm/s
    float m_defaultDensity = 0.001f; // 1 g/cm^3
    float m_minTimestep = 16.667f; // 60 fps
    float m_maxTimestep = 33.333f; // 30 fps

    bool m_running = true;
    bool m_forceDebugDraw = false;
    // For performance, used to keep track if we have indiviually enabled debug drawing for any
    // collision shape
    bool m_hasIndividualDebugDraw = false;
    bool m_physicsInitialized = false;
    bool m_enableCCD = false;

    PhysXWorld *m_physx = nullptr;
    QQuick3DViewport *m_sceneView = nullptr;
    QQuick3DDefaultMaterial *m_debugMaterial = nullptr;

    friend class QQuick3DPhysicsMesh; // TODO: better internal API
    friend class QTriangleMeshShape; //####
    friend class QHeightFieldShape;
    friend class QQuick3DPhysicsHeightField;
    static QDynamicsWorld *self;
    static physx::PxPhysics *getPhysics();
    static physx::PxCooking *getCooking();
    physx::PxCooking *cooking();
    void findSceneView();
    QThread m_workerThread;
};

QT_END_NAMESPACE

#endif // DYNAMICSWORLD_H
