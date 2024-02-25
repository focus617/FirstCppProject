#pragma once

#include "tools/maths/internal/TransformNotifier.hpp"
#include "tools/maths/FQuaternion.hpp"
#include "tools/maths/FMatrix4.hpp"
#include "tools/maths/FVector3.hpp"

namespace xuzy::Maths
{
	/**
	* @brief Mathematic representation of a 3D transformation with float
	* precision
	*/
	class FTransform
	{
	public:
		/**
		* @brief @brief Create a transform without setting a parent
		* @param p_localPosition
		* @param p_localRotation
		* @param p_localScale
		*/
		FTransform(FVector3 p_localPosition = FVector3(0.0f, 0.0f, 0.0f), FQuaternion p_localRotation = FQuaternion::Identity, FVector3 p_localScale = FVector3(1.0f, 1.0f, 1.0f));

		/**
		* @brief Destructor of the transform.
		* Will notify children on destruction
		*/
		~FTransform();

		/**
		* @brief Simple callback that will treat parent notifications
		* @param p_notification
		*/
		void NotificationHandler(Internal::TransformNotifier::ENotification p_notification);

		/**
		* @brief Defines a parent to the transform
		* @param p_parent
		*/
		void SetParent(FTransform& p_parent);

		/**
		* @brief Set the parent to nullptr and recalculate world matrix
		* Returns true on success
		*/
		bool RemoveParent();

		/**
		* @brief Check if the transform has a parent
		*/
		bool HasParent() const;

		/**
		* @brief Initialize transform with raw data from world info
		* @param p_position
		* @param p_rotation
		* @param p_scale
		*/
		void GenerateMatricesWorld(FVector3 p_position, FQuaternion p_rotation, FVector3 p_scale);

		/**
		* @brief Initialize transform with raw data from local info
		* @param p_position
		* @param p_rotation
		* @param p_scale
		*/
		void GenerateMatricesLocal(FVector3 p_position, FQuaternion p_rotation, FVector3 p_scale);

		/**
		* @brief Re-update world matrix to use parent transformations
		*/
		void UpdateWorldMatrix();

		/**
		* @brief Re-update local matrix to use parent transformations
		*/
		void UpdateLocalMatrix();

		/**
		* @brief Set the position of the transform in the local space
		* @param p_newPosition
		*/
		void SetLocalPosition(FVector3 p_newPosition);

		/**
		* @brief Set the rotation of the transform in the local space
		* @param p_newRotation
		*/
		void SetLocalRotation(FQuaternion p_newRotation);

		/**
		* @brief Set the scale of the transform in the local space
		* @param p_newScale
		*/
		void SetLocalScale(FVector3 p_newScale);

		/**
		* @brief Set the position of the transform in world space
		* @param p_newPosition
		*/
		void SetWorldPosition(FVector3 p_newPosition);

		/**
		* @brief Set the rotation of the transform in world space
		* @param p_newRotation
		*/
		void SetWorldRotation(FQuaternion p_newRotation);

		/**
		* @brief Set the scale of the transform in world space
		* @param p_newScale
		*/
		void SetWorldScale(FVector3 p_newScale);

		/**
		* @brief Translate in the local space
		* @param p_translation
		*/
		void TranslateLocal(const FVector3& p_translation);

		/**
		* @brief Rotate in the local space
		* @param p_rotation
		*/
		void RotateLocal(const FQuaternion& p_rotation);

		/**
		* @brief Scale in the local space
		* @param p_scale
		*/
		void ScaleLocal(const FVector3& p_scale);

		/**
		* @brief Return the position in local space
		*/
		const FVector3& GetLocalPosition() const;

		/**
		* @brief Return the rotation in local space
		*/
		const FQuaternion& GetLocalRotation() const;

		/**
		* @brief Return the scale in local space
		*/
		const FVector3& GetLocalScale() const;

		/**
		* @brief Return the position in world space
		*/
		const FVector3& GetWorldPosition() const;

		/**
		* @brief Return the rotation in world space
		*/
		const FQuaternion& GetWorldRotation() const;

		/**
		* @brief Return the scale in world space
		*/
		const FVector3& GetWorldScale() const;

		/**
		* @brief Return the local matrix
		*/
		const FMatrix4& GetLocalMatrix() const;

		/**
		* @brief Return the world matrix
		*/
		const FMatrix4& GetWorldMatrix() const;

		/**
		* @brief Return the transform world forward
		*/
		FVector3 GetWorldForward() const;

		/**
		* @brief Return the transform world up
		*/
		FVector3 GetWorldUp() const;

		/**
		* @brief Return the transform world right
		*/
		FVector3 GetWorldRight() const;

		/**
		* @brief Return the transform local forward
		*/
		FVector3 GetLocalForward() const;

		/**
		* @brief Return the transform local up
		*/
		FVector3 GetLocalUp() const;

		/**
		* @brief Return the transform local right
		*/
		FVector3 GetLocalRight() const;

	public:
		Internal::TransformNotifier Notifier;
		Internal::TransformNotifier::NotificationHandlerID m_notificationHandlerID;

	private:
		void PreDecomposeWorldMatrix();
		void PreDecomposeLocalMatrix();

		/* Pre-decomposed data to prevent multiple decomposition */
		FVector3 m_localPosition;
		FQuaternion m_localRotation;
		FVector3 m_localScale;
		FVector3 m_worldPosition;
		FQuaternion m_worldRotation;
		FVector3 m_worldScale;

		FMatrix4 m_localMatrix;
		FMatrix4 m_worldMatrix;

		FTransform*	m_parent;
	};
}