# Hard rows

The dead-end log. Read the entry for your RVA before retrying; an old result holds only for its source,
headers and compiler profile. To add one, append a row or section with the RVA, what you tried, the outcome,
and the smallest open question. No disassembly dumps.

## Evidence to carry into the next attempt

| Case and reference RVA | Witness and next distinction | Record date |
| --- | --- | --- |
| `ActionManager` shroud predicate `0x0041B85D` and callers `0x0041BA1C`, `0x0041B9B0` | The reference passes object arguments in registers. Prior matching source uses file-local linkage and ordinary flags. A trial unit-wide optimization change cost existing matches. Investigate the actual unit and caller context. | 2026-11-17 |
| `SpecialAbilityUpdateModuleData` constructor `0x0044EB54` and `SpecialAbilityUpdate` destructor `0x00451F45` | The early constructor trial reproduced member stores while exception-state and floating-point ordering still differed. The destructor's teardown target supports a plain-allocator `std::list<ObjectID>` at the affected field. A later trial recorded a match with `/EHs /EHc-` for the destructor. Check the current file and compiler profile before treating either historical mismatch as open. | 2026-11-17 and 2026-11-18 recovery notes |
| `FullFadeTransition` and `CountUpTransition` constructors `0x0035F2B3`, `0x0035F64A` | The reference stores the base, frame data, table identity and drawing state in an observed order. Constructor variants moved the drawing-state initialization earlier. Check conditions in inherited and nested initialization. | 2026-11-17 |
| `TerrainTypeCollection::findTerrain` `0x00317D18` | The reference epilogue shows the by-value string's cleanup. Shared `StringBase::compare` exception semantics affect the frame and branch shape. Check its declaration against other callers before changing it. | 2026-11-17 |
| `CastleBehavior::getCampDiameter` `0x00396E92` | The compiler reuses the incoming player parameter's stack slot for the loop index. Hoisting the count or naming the side reference narrowed the mismatch to frame layout and lifetime. | 2026-11-17 |
| `CastleBehavior` destructor `0x0039857D` | The trial reproduced map, string, set and vector member order. The allocator call form and exception state differed. Trace the actual vector allocator contract. | 2026-11-17 |
| `MemoryPool::_AddHeap` `0x000306B0` | The difference after the existing-heap case is in final bucket insertion scheduling. Moving the increment and changing individual stores under the evidenced unit profile gave different register schedules. Keep the shared allocation model. | 2026-11-17 |
| `ScriptConditions::evaluateDistanceBetweenObjects` `0x003E4C8D` | The reference uses immediate indices for constant parameters and one result after the switch. The accessor already emits the expected comparisons in another caller. Compare coordinate copy and result lifetime before changing the accessor. | 2026-11-17 |
| `EAStringC` concatenation `0x006D4F70`, `0x006D5160` | A source trial emits the expected bodies with both the copy constructor and `Reserve` visible. Each single-helper trial left a mismatch. The byte trial leaves original helper membership and data reference proof open. | 2026-11-18 |
| Named command-button search actions `0x003BD905`, `0x003BDC65` | Compare the option-bit branch, register allocation and filter lifetime with the related landed wrappers. Confirm the action's own registration name before adopting a spelling. | 2026-11-18 |

### Stealth detector update

The reconstructed source for StealthDetectorUpdate::update (RVA 0x004A2D93) is
omitted pending a matching implementation. Its ledger row remains open.

The previous body declared a string in a branch that never ran. The declaration
affected compiler stack allocation. With the declaration removed, five ordinary
source forms emitted the same 1,579-byte extent and the same 87 relocation
offsets. The save and reload of the receiver at offsets +0x1A and +0xE8 use
EBP-0x18 in those builds and EBP-0x1C in the reference. The associated 109 bytes
of cleanup code and 156 bytes of exception data matched after resolving
relocations. A sixth form that used a reference to a live sound-name temporary
emitted 1,583 bytes and introduced other differences. None of these forms
qualifies as a byte match.

Corresponding source without the dead declaration is included below for
comparison. It is a reconstruction candidate.

```cpp
UpdateSleepTime StealthDetectorUpdate::update()
{
	const StealthDetectorUpdateModuleData *data = static_cast<const StealthDetectorUpdateModuleData *>(m_moduleData);
	Object *self = getObject();
	if (self->testStatus(OBJECT_STATUS_PHANTOM_STRUCTURE))
		return UPDATE_SLEEP_NONE;
	if (self->isEffectivelyDead())
		return UPDATE_SLEEP_FOREVER;
	if (self->testStatus(OBJECT_STATUS_UNDER_CONSTRUCTION))
		return UPDATE_SLEEP_NONE;
	if (self->testStatus(OBJECT_STATUS_SOLD))
		return UPDATE_SLEEP_FOREVER;

	Object *containedBy = self->getContainedBy();
	if (containedBy)
	{
		ContainModuleInterface *contain = containedBy->getContain();
		if (contain)
		{
			if (contain->isGarrisonable())
			{
				if (!data->m_garrison)
					return (UpdateSleepTime)data->m_updateRate;
			}
			else if (!data->m_transport)
				return (UpdateSleepTime)data->m_updateRate;
		}
	}

	if (data->m_upgradeName.compare(AsciiString::TheEmptyString) != 0)
	{
		const UpgradeTemplate *upgrade = TheUpgradeCenter->findUpgrade(data->m_upgradeName);
		if (!upgrade || !self->hasUpgrade(upgrade))
			return UPDATE_SLEEP_NONE;
	}

	float range = self->getVisionRange();
	if (data->m_detectionRange > 0.0f)
		range = data->m_detectionRange;

	bool foundSomeone = false;
	ObjectIterator iter = ThePartitionManager->iterateObjectsInRange(&self->m_cachedPos, range, 0,
		PartitionFilterRelationship(self, PartitionFilterRelationship::ALLOW_ENEMIES | PartitionFilterRelationship::ALLOW_NEUTRAL, false)
			.link(PartitionFilterAcceptByKindOf(data->require(), data->forbid())
				.link(Rva00261513Filter(self, true, range)
					.link(&PartitionFilterSameMapStatus(getObject())))), 0);
	for (Object *them = iter.next(); them; them = iter.next())
	{
		if (them->isEffectivelyDead())
			continue;

		bool forceUnstealth;
		if (them->rva0028F4EF() == 2 && !them->testModelConditionState(MODELCONDITION_DISGUISED))
			forceUnstealth = false;
		else
			forceUnstealth = true;

		if (them->testModelConditionState(MODELCONDITION_DISGUISED))
		{
			static NameKeyType key_SpecialDisguiseUpdate = TheNameKeyGenerator->nameToKey("SpecialDisguiseUpdate");
			SpecialDisguiseUpdate *disguise = static_cast<SpecialDisguiseUpdate *>(them->findUpdateModule(key_SpecialDisguiseUpdate));
			if (disguise)
				disguise->rva004B05F5(false);
		}
		if (forceUnstealth && !static_cast<unsigned char>(them->rva002933CD()) && !them->testStatus(OBJECT_STATUS_DETECTED))
			forceUnstealth = false;
		else
			forceUnstealth = true;

		TheGameLogic->getManager178()->detected(them, self, data->m_updateRate + 1, 2);
		Rva00373EC6 *stealth = them->rva0028F4BC();
		if (stealth && them->testStatus(OBJECT_STATUS_CAN_STEALTH))
		{
			foundSomeone = true;
			stealth->markAsDetected(data->m_updateRate + 1, 2, self, true);
			if (data->m_cancelRing && stealth->m_31)
				stealth->rva00374815();

			if (data->grid())
			{
				ParticleSystemHandle sys = TheParticleSystemManager->createParticleSystem(data->grid(), true);
				if (sys)
				{
					Coord3D gridPosition;
					gridPosition.x = them->m_cachedPos.x;
					gridPosition.y = them->m_cachedPos.y;
					gridPosition.z = self->m_cachedPos.z + 17.0f;
					gridPosition.x -= static_cast<int>(gridPosition.x) % 12;
					gridPosition.y -= static_cast<int>(gridPosition.y) % 12;
					sys.m_system->setPosition(&gridPosition);
				}
			}
		}
		else
		{
			ContainModuleInterface *contain = them->getContain();
			if (contain && contain->isGarrisonable() && contain->getStealthUnitsContained())
			{
				ContainmentListView view = contain->getList();
				for (_STL::list<Object *>::const_iterator item = view.objects().begin(); item != view.objects().end(); ++item)
				{
					Object *rider = *item;
					Rva00373EC6 *riderStealth = rider->rva0028F4BC();
					if (riderStealth)
					{
						foundSomeone = true;
						if (self->getControllingPlayer() != rider->getControllingPlayer() && self->getRelationship(rider) != ALLIES)
						{
							riderStealth->markAsDetected(data->m_updateRate + 2, 2, self, true);
							TheGameLogic->getManager178()->detected(rider, self, data->m_updateRate + 2, 2);
						}
					}
				}
			}
		}
		if (!forceUnstealth)
		{
			Drawable *draw = them->Thing::getDrawable();
			if (draw)
				draw->m_secondMaterialPassOpacity = 1.0f;
		}
	}

	if (data->grid() && self->getShroudStatusForPlayer(ThePlayerList->m_local->m_playerIndex) <= 2)
	{
		Drawable *draw = self->Thing::getDrawable();
		Coord3D bone;
		bone.x = -1.66f;
		bone.y = 5.5f;
		bone.z = 15.0f;
		if (draw)
			draw->getPristineBonePositions(data->bone().str(), 0, &bone, 0, 1, 0);

		const ParticleSystemTemplate *ping = foundSomeone ? data->bright() : data->ping();
		if (ping)
		{
			ParticleSystemHandle sys = TheParticleSystemManager->createParticleSystem(ping, true);
			if (sys)
			{
				if (draw)
					sys.m_system->attachToDrawable(draw);
				else
					sys.m_system->attachToObject(self);
				sys->setPosition(&bone);
			}
		}
		if (data->beacon())
		{
			ParticleSystemHandle sys = TheParticleSystemManager->createParticleSystem(data->beacon(), true);
			if (sys)
			{
				if (draw)
					sys.m_system->attachToDrawable(draw);
				else
					sys.m_system->attachToObject(self);
				sys->setPosition(&bone);
			}
		}

		AudioEventRTS sound(OpaqueRefElement4(), INVALID_ID);
		if (foundSomeone)
			sound.assignSound(data->loudRef());
		else
			sound.assignSound(data->soundRef());
		sound.setObjectID(self->m_id);
		TheAudio->addAudioEvent(&sound);
	}
	return (UpdateSleepTime)data->m_updateRate;
}
```

### W3DScene function placement

| RVA | Observed role | Evidence still needed |
| --- | --- | --- |
| 0x0006EDE3 | SceneClass virtual slot 8 and five network call sites select the same ten-byte body. Another copy at 0x0065D7A0 emits the same bytes. | The original compiler contribution that distinguishes the selected scene and network uses from the other copy. |
| 0x0006F039 | A caller at 0x00265B6A and the field at Object offset 0x10C establish its use in testing model condition flags. | The original object-file contribution selecting this 37-byte body and a placement that preserves both callable names. |
| 0x0006F3CC | A subsystem setter takes AsciiString by value. The assignment and destructor calls and exception sidecars establish the copy and cleanup. | The original compiler contribution selecting this 52-byte inline copy and its exception sidecars. |

### Frame command resend

FrameData::allCommandsReady (RVA 0x005DA563) walks the command list before
resetting the frame and requesting a resend. The reference executable follows
the next link and tests it at 0x005DA598 through 0x005DA59D. The traversal
does not retain a result. Removing it emits 65 bytes for a 75-byte reference
body and changes both the function match and the object verification. The
source retains that traversal.

## Resolved examples

- **`StructureCollapseUpdateModuleData` destructor** (`0x00256B94`).
  The implicit destructor produces member cleanup and the trailing virtual
  table store. A hand-written destructor adds an earlier store. Recovery
  notes, 2026-11-17, change `0a099f94`.
- **`ModuleFactory::newModuleDataFromINI`** (`0x002571F9`). A vector of
  pointers to const module data is the owning container. Correcting its shared
  type fixes the emitted use of the surviving pointer across `push_back`.
  Recovery notes, 2026-11-17, change `0fe76f7d`.
- **`AptPlayer` call helper** (`0x00222B19`). The recovered function
  matches. Other listbox helpers in the same investigation show a file-local
  compiler convention when the caller is present. Recovery notes,
  2026-11-18, change `167dd10c83`.
- **`GameFileClass::Create` and `Delete`**. Both header-inline
  definitions need the true-returning fold selected by their own virtual
  slots. The controlled Fords of Isen II test on 2026-11-18 restored the
  measured reference checksums with those two returns corrected.

Keep disassembly in your local reference environment. Report the
source-level difference, address and checker finding.
