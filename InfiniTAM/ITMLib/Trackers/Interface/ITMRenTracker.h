// Copyright 2014-2015 Isis Innovation Limited and the authors of InfiniTAM

#pragma once

#include "ITMTracker.h"
#include "../Base/ITMImageHierarchy.h"
#include "../Base/ITMTemplatedHierarchyLevel.h"
#include "../Base/TrackerIterationType.h"
#include "../../LowLevel/Interface/ITMLowLevelEngine.h"
#include "../../Scene/ITMScene.h"

namespace ITMLib
{
	/** Base class for engine performing SDF based depth tracking.
	*/
	template<class TVoxel, class TIndex>
	class ITMRenTracker : public ITMTracker
	{
	private:
		ITMTrackingState *trackingState; 
		const ITMLowLevelEngine *lowLevelEngine;

		ITMFloatImage *tempImage1, *tempImage2;

		const ITMView *view;

		int *noIterationsPerLevel;

		void PrepareForEvaluation(const ITMView *view);

	protected:
		const ITMScene<TVoxel, TIndex> *scene;
		ITMImageHierarchy<ITMTemplatedHierarchyLevel<ITMFloat4Image> > *viewHierarchy;

		int levelId;
		bool rotationOnly;

		float hessian[6 * 6];
		float nabla[6];

		virtual void F_oneLevel(float *f, Matrix4f invM) = 0;
		virtual void G_oneLevel(float *gradient, float *hessian, Matrix4f invM) const = 0;

		virtual void UnprojectDepthToCam(ITMFloatImage *depth, ITMFloat4Image *upPtCloud, const Vector4f &intrinsic) = 0;

	public:

		void applyDelta(const ITMPose & para_old, const float *delta, ITMPose & para_new) const;
		int numParameters(void) const { return 6; }

		void TrackCamera(ITMTrackingState *trackingState, const ITMView *view);

		bool requiresColourRendering(void) const { return false; }
		bool requiresDepthReliability(void) const { return false; }

		ITMRenTracker(Vector2i imgSize, TrackerIterationType *trackingRegime, int noHierarchyLevels, const ITMLowLevelEngine *lowLevelEngine, 
			const ITMScene<TVoxel, TIndex> *scene, MemoryDeviceType memoryType);

		virtual ~ITMRenTracker(void);
	};
}
