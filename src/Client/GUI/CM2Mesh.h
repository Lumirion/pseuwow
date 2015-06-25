#ifndef __M2_MESH_H_INCLUDED__
#define __M2_MESH_H_INCLUDED__

#include "irrlicht/irrlicht.h"
#include <limits>

namespace irr
{
namespace scene
{

    struct M2Animation
    {
        f32 probability;
        u32 begin;
        u32 end;
    };
	class IAnimatedMeshSceneNode;
	class IBoneSceneNode;

	class CM2Mesh: public ISkinnedMesh
	{
	public:

		//! constructor
		CM2Mesh();

		//! destructor
		virtual ~CM2Mesh();

		//! returns the amount of frames. If the amount is 1, it is a static (=non animated) mesh.
		virtual u32 getFrameCount() const;

        //! Gets the default animation speed of the animated mesh.
        /** \return Amount of frames per second. If the amount is 0, it is a static, non animated mesh. */
        virtual f32 getAnimationSpeed() const;

        //! Gets the frame count of the animated mesh.
        /** \param fps Frames per second to play the animation with. If the amount is 0, it is not animated.
        The actual speed is set in the scene node the mesh is instantiated in.*/
        virtual void setAnimationSpeed(f32 fps);

		//! returns the animated mesh based on a detail level (which is ignored)
		virtual IMesh* getMesh(s32 frame, s32 detailLevel=255, s32 startFrameLoop=-1, s32 endFrameLoop=-1);

		//! Animates this mesh's joints based on frame input
		//! blend: {0-old position, 1-New position}
		virtual void animateMesh(f32 frame, f32 blend);

		//! Preforms a software skin on this mesh based of joint positions
		virtual void skinMesh();

		//! returns amount of mesh buffers.
		virtual u32 getMeshBufferCount() const;

		//! returns pointer to a mesh buffer
		virtual IMeshBuffer* getMeshBuffer(u32 nr) const;

		//! Returns pointer to a mesh buffer which fits a material
 		/** \param material: material to search for
		\return Returns the pointer to the mesh buffer or
		NULL if there is no such mesh buffer. */
		virtual IMeshBuffer* getMeshBuffer( const video::SMaterial &material) const;

		//! returns an axis aligned bounding box
		virtual const core::aabbox3d<f32>& getBoundingBox() const;

		//! set user axis aligned bounding box
		virtual void setBoundingBox( const core::aabbox3df& box);

		//! sets a flag of all contained materials to a new value
		virtual void setMaterialFlag(video::E_MATERIAL_FLAG flag, bool newvalue);

		//! set the hardware mapping hint, for driver
		virtual void setHardwareMappingHint(E_HARDWARE_MAPPING newMappingHint, E_BUFFER_TYPE buffer=EBT_VERTEX_AND_INDEX);

		//! flags the meshbuffer as changed, reloads hardware buffers
		virtual void setDirty(E_BUFFER_TYPE buffer=EBT_VERTEX_AND_INDEX);

        //! updates the bounding box
        virtual void updateBoundingBox(void);

        //! Returns the type of the animated mesh.
		virtual E_ANIMATED_MESH_TYPE getMeshType() const;

		//! Gets joint count.
		virtual u32 getJointCount() const;

		//! Gets the name of a joint.
		virtual const c8* getJointName(u32 number) const;

		//! Gets a joint number from its name
		virtual s32 getJointNumber(const c8* name) const;

		//! uses animation from another mesh
		virtual bool useAnimationFrom(const ISkinnedMesh *mesh);

		//! Update Normals when Animating
		//! False= Don't (default)
		//! True = Update normals, slower
		virtual void updateNormalsWhenAnimating(bool on);

		//! Sets Interpolation Mode
		virtual void setInterpolationMode(E_INTERPOLATION_MODE mode);

		//! Recovers the joints from the mesh
		virtual void recoverJointsFromMesh(core::array<IBoneSceneNode*> &JointChildSceneNodes);

		//! Tranfers the joint data to the mesh
		virtual void transferJointsToMesh(const core::array<IBoneSceneNode*> &JointChildSceneNodes);

		//! Tranfers the joint hints to the mesh
		virtual void transferOnlyJointsHintsToMesh(const core::array<IBoneSceneNode*> &JointChildSceneNodes);

		//! Creates an array of joints from this mesh
		virtual void createJoints(core::array<IBoneSceneNode*> &JointChildSceneNodes,
				IAnimatedMeshSceneNode* AnimatedMeshSceneNode,
				ISceneManager* SceneManager);

		//! Convertes the mesh to contain tangent information
		virtual void convertMeshToTangents();

		//! Does the mesh have no animation
		virtual bool isStatic();

		//! (This feature is not implemented in irrlicht yet)
		virtual bool setHardwareSkinning(bool on);

		//Interface for the mesh loaders (finalize should lock these functions, and they should have some prefix like loader_

		//these functions will use the needed arrays, set vaules, etc to help the loaders

		//! exposed for loaders to add mesh buffers
		virtual core::array<SSkinMeshBuffer*> &getMeshBuffers();

		//! alternative method for adding joints
		virtual core::array<SJoint*> &getAllJoints();

		//! alternative method for adding joints
		virtual const core::array<SJoint*> &getAllJoints() const;

		//! loaders should call this after populating the mesh
		virtual void finalize();

        SSkinMeshBuffer *addMeshBuffer(u32 id)
        {
          GeoSetID.push_back(id);
          GeoSetRender.push_back((id==0?true:false));//This may be changed later on when we know more about the submesh switching business
          return addMeshBuffer();
        };
		virtual SSkinMeshBuffer *addMeshBuffer();

		virtual SJoint *addJoint(SJoint *parent=0);

		virtual SPositionKey *addPositionKey(SJoint *joint);
		virtual SRotationKey *addRotationKey(SJoint *joint);
		virtual SScaleKey *addScaleKey(SJoint *joint);

		virtual SWeight *addWeight(SJoint *joint);

        //Retrieve animation information
        void getFrameLoop(u32 animId, s32 &start, s32 &end);
        void newAnimation(u32 id, s32 start, s32 end, f32 probability);
        //Retrieve geoset rendering information
        void setGeoSetRender(u32 id, bool render);
        void setMBRender(u32 id, bool render);
        bool getGeoSetRender(u32 meshbufferNumber);
		//Submesh Array containing data for each submesh sorted for proper render order
		struct BufferInfo{  //Submesh Data Element
			u16 ID;
			u16 Mode;
			s16 order;
			u16 block;
			u16 blend;
			u16 flag;       // Renderflag
			u16 unknown;    // still no clue what this is
			bool solid;
			bool animatedtexture;
			core::vector3df Coordinates; // Center point of an aabb
			float Back;     // Far Edge
			float Middle;   // Center
			float Front;    // Near Edge
			float SortPoint; // We set this to back middle or front
			float Radius;
			float Xpos;     // Right Edge
			float Xneg;     // Left Edge
			float Ypos;     // Top Edge
			float Yneg;     // Bottom Edge
		};
		core::array<BufferInfo> BufferMap; // The Array to Map out my Submesh Data Element order
		//add light nodes
		struct Lights{
			u16 Type;
			s16 Bone;
			core::vector3df Position;  //if these are to be animated then at the end of this struct should be arrays for the timestamps
			video::SColorf AmbientColor;  // AmbientIntensity is in AmbientColor.a;
			video::SColorf DiffuseColor;  // DiffuseIntensity is in DiffuseColor.a;
			float AttenuationStart;
			float AttenuationEnd;
			u32 Unknown;  //probably wont use this since wowdev doesn't know what it is for. may be for enable shadows since its usualy 1
		};

		core::array<Lights> M2MLights;
		void attachM2Lights(IAnimatedMeshSceneNode *Node, ISceneManager *smgr); //lumirion's test
		
		// global textures
		core::array<io::path> Textures;

		// .Skins views
		struct texture{
			u16 TextureNumber; // a submesh can have up to 3 textures
			u16 Path;  //  pointer into global list of texure paths&names cm2mesh::Textures
			bool animated;     
			u16 RenderFlag; 
			u16 BlendFlag;
			s16 shaderType;   // actualy 2 u8 shader flags
			u16 Mode;  // helps indicate shader?  opcount:: texture, UV animation, render flags, and transparency indices in this stuct point to start index opcount/mode = how many values to get
			u16 Block; // = some sort of render flag indicating submesh grouping&order
			//pointers to animations
			u16 VertexColor;  // needs to point directly into cm2mesh::VertexColor or =-1 for no color
			u16 transparency; // needs to = transparencylookup[textureunit.transparency] to point into the global CM2Mesh::Transparency
			u16 uvanimation;  // needs to = uvanimationlookup[textureunit.texAnimIndex] to point into the global CM2Mesh::UVAnimations
		};
		struct submesh{
			u32 MeshPart;             // indcates head or hand or other parts.  Could do this as a string
			u16 RootBone;             // index to bone/joint this submesh attaches to
			float Radius;
			float Distance;           // distance between built in camera and submeshe's nearest vertex
			u16 NearestVertex;        // index to this submeshes vertex nearest the camera
			core::stringc UniqueName; // id for the submesh incase submesh's geometry exists in multiple .skins.  Id should look like StartVertex_EndVertex.  maybe list bones for this submesh too
			u16 SubmeshIndex;         // index to this submesh's data in the .skin file and CM2MeshFileLoader's arrays
			u16 BufferIndex;          // index to this submesh's buffer in this mesh
			irr::core::array <texture> Textures; // this submesh's texture descriptions (should be limited to 3)
		};
		struct skin{
			u16 ID; // xx.skin id like 0, 1, 2 etc...  // don't need this its redundant. It is the same as this skin's index in Skins
			irr::core::array <submesh> Submeshes; // submeshes in this view
		};

		core::array<skin> Skins;
		u32 SkinID; // points to active skin.  This belongs in CM2MeshSceneNode not here
		void LinkChildMeshes(IAnimatedMeshSceneNode *UI_ParentMeshNode, ISceneManager *smgr, core::array<IBoneSceneNode*> &JointChildSceneNodes); // use this for UI (scene) meshes
		// ToDo:: add an array of unique preconfigured materials to replace part of texture element new texture element will index its material
		// ToDo:: add a function to return a list of visible submeshes for the scenenode to render. All possible submeshes for all views should be added to the mesh

		////////////////////
		// submesh extream points
		////////////////////
		struct BufferPointGroup                            // A structure to store a group of indexes to the extream points of a single submesh buffer
		{
			core::array <u32> indexes;
		};
		core::array <BufferPointGroup> ExtreamPointGroups;  // Holds all the groups of extremes for all the submesh buffers
		void findExtremes();
		void getDist_NearandFar_ofSubmesh(u32 submeshID, float &near, float &far, const core::vector3df camPos, core::vector3df camNormal); // gets the distance to nearest and farthest point of a submesh from a camera plane's origen

private:

		void checkForAnimation();

		void normalizeWeights();

		void buildAllAnimatedMatrices(SJoint *Joint=0, SJoint *ParentJoint=0); //public?

		void getFrameData(f32 frame, SJoint *Node,
				core::vector3df &position, s32 &positionHint,
				core::vector3df &scale, s32 &scaleHint,
				core::quaternion &rotation, s32 &rotationHint);

		void CalculateGlobalMatrices(SJoint *Joint,SJoint *ParentJoint);

		void SkinJoint(SJoint *Joint, SJoint *ParentJoint);

		void calculateTangents(core::vector3df& normal,
			core::vector3df& tangent, core::vector3df& binormal,
			core::vector3df& vt1, core::vector3df& vt2, core::vector3df& vt3,
			core::vector2df& tc1, core::vector2df& tc2, core::vector2df& tc3);


		core::array<SSkinMeshBuffer*> *SkinningBuffers; //Meshbuffer to skin, default is to skin localBuffers

		core::array<SSkinMeshBuffer*> LocalBuffers;
        core::array<u32> GeoSetID; //Array of Submesh Meshpart IDs used for switching Geosets on and off
        core::array<bool> GeoSetRender;

        core::array<SJoint*> AllJoints;
		core::array<SJoint*> RootJoints;

		bool HasAnimation;

		bool PreparedForSkinning;

		f32 AnimationFrames;
        f32 FramesPerSecond;

		f32 LastAnimatedFrame;
        bool SkinnedLastFrame;

		bool AnimateNormals;

		bool HardwareSkinning;


		E_INTERPOLATION_MODE InterpolationMode;

		core::aabbox3d<f32> BoundingBox;

		core::array< core::array<bool> > Vertices_Moved;

        core::array< M2Animation > Animations;
        core::map<u32, core::array<u32> > AnimationLookup;

		// A structure to track a Group of vetex Points in a submesh.
		// It contains functions to subdivide and find the extreme points of a submesh.
		// Adapted from ogre3d 
		struct Group_of_Points
		{
			core::aabbox3d<float> Box;
			core::list<u32> mIndices;

			Group_of_Points ()
			{ }

			bool empty () const
			{
				if (mIndices.empty ())
					return true;
				if (Box.MinEdge == Box.MaxEdge)
					return true;
				return false;
			}

			float volume () const
			{
				return (Box.MaxEdge.X - Box.MinEdge.X) * (Box.MaxEdge.Y - Box.MinEdge.Y) * (Box.MaxEdge.Z - Box.MinEdge.Z);
			}
        
			float getValueByAxis_FromVector(u32 Axis, core::vector3df V)
			{
				if(Axis==1){return V.X;}
				else if(Axis==2){return V.Y;}
				else if(Axis==3){return V.Z;}
			}

			void extend (core::vector3df v)
			{
				if (v.X < Box.MinEdge.X) Box.MinEdge.X = v.X;
				if (v.Y < Box.MinEdge.Y) Box.MinEdge.Y = v.Y;
				if (v.Z < Box.MinEdge.Z) Box.MinEdge.Z = v.Z;
				if (v.X > Box.MaxEdge.X) Box.MaxEdge.X = v.X;
				if (v.Y > Box.MaxEdge.Y) Box.MaxEdge.Y = v.Y;
				if (v.Z > Box.MaxEdge.Z) Box.MaxEdge.Z = v.Z;
			}

			void computeBBox (scene::IMeshBuffer *submesh) // resize box around its vertices
			{
				// make the box imposibly small
				Box.MinEdge.X = Box.MinEdge.Y = Box.MinEdge.Z = std::numeric_limits<float>::infinity(); // set near value imposibly far out of range
				Box.MaxEdge.X = Box.MaxEdge.Y = Box.MaxEdge.Z = -std::numeric_limits<float>::infinity(); // set the far value imposibly near out of range

				for (core::list<u32>::Iterator i = mIndices.begin (); i != mIndices.end (); ++i)
				{
					extend (submesh->getPosition(*i)); // expand box to include the vertex indicated by current Index 
				}
			}

			Group_of_Points split (u32 split_axis, scene::IMeshBuffer *submesh)
			{
				// Generate a float halfway along the current axis 
				float SplitPoint = (getValueByAxis_FromVector(split_axis, Box.MinEdge) + getValueByAxis_FromVector(split_axis, Box.MaxEdge)) * 0.5f;
				Group_of_Points newbox;

				// Separate Vertices into two groups by what side of the SplitPoint they fall on using the current box and the newbox.  
				for (core::list<u32>::Iterator i = mIndices.begin (); i != mIndices.end (); )
				{
					if (getValueByAxis_FromVector(split_axis, submesh->getPosition(*i)) > SplitPoint) // if the vertex belongs in the new box
					{
						newbox.mIndices.push_back(*i); // copy it to the new box
						mIndices.erase(i); // and delete it from the original
					}
					else
						++i; // if it belongs where it is move along to the next vertex
				}

				computeBBox (submesh);
				newbox.computeBBox (submesh);

				return newbox;
			}
		};

	};

} // end namespace scene
} // end namespace irr

#endif



