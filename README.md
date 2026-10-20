# BattleBlaster

Learning project for Unreal Engine 5.6 — a top-down tank game: drive a tank, aim the
turret with the mouse and destroy all enemy towers to clear the level. Built to practice
C++ in UE: `APawn` class hierarchies, components, Enhanced Input, projectiles and damage,
`GameMode`/`GameInstance`, timers, UMG widgets, Niagara effects, sounds and camera shake.

Started from a blank C++ project (`Source/BattleBlaster/`), with Blueprint wrappers,
input assets and levels in `Content/`.

## Custom code

`Source/BattleBlaster/BasePawn.h/.cpp` — the shared base `APawn` for the tank and the
towers:

- components: `CapsuleComp` (root) → `BaseMesh` → `TurretMesh` → `ProjectileSpawnPoint`
- `RotateTurret(FVector)` — smoothly turns the turret toward a target (yaw only, via
  `FMath::RInterpTo`)
- `Fire()` — spawns `ProjectileClass` at `ProjectileSpawnPoint` and sets itself as the
  projectile's owner
- `HandleDestruction()` — plays the death effects: `DeathEffect` (Niagara),
  `ExplodeSound`, `DeathCameraShakeClass`

`Source/BattleBlaster/Tank.h/.cpp` — the player pawn (`ABasePawn`):

- `SpringArmComp` + `CameraComp` for the top-down camera
- Enhanced Input: `DefaultMappingContext`, `MoveAction`, `TurnAction`, `FireAction`;
  `MoveInput()`/`TurnInput()` move and turn the tank with `Speed`/`TurnRate`
- `Tick()` rotates the turret toward the cursor (`GetHitResultUnderCursor`)
- `SetPlayerEnabled(bool)` — enables/disables input and the mouse cursor (input is off
  until the start countdown ends)
- `HandleDestruction()` hides the tank and disables it instead of destroying it;
  `GetIsAlive()` lets the towers stop firing

`Source/BattleBlaster/Tower.h/.cpp` — an enemy turret (`ABasePawn`): turns toward the
tank while it's within `FireRange` and fires every `FireRate` seconds on a looping
timer (`CheckFireCondition()`); destroyed in `HandleDestruction()`.

`Source/BattleBlaster/Projectile.h/.cpp` — a projectile (`AActor`) with
`UProjectileMovementComponent` and a Niagara trail (`TrailParticles`):

- `OnHit()` (bound to `OnComponentHit`) applies `Damage` via
  `UGameplayStatics::ApplyDamage` to anything except its owner, then destroys itself
- effects: `LaunchSound` on spawn; `HitParticles` and `HitCameraShakeClass` on hit

`Source/BattleBlaster/HealthComponent.h/.cpp` — an `ActorComponent` with
`MaxHealth`/`Health`; subscribes to the owner's `OnTakeAnyDamage` and reports
`ActorDied()` to the game mode when health reaches zero.

`Source/BattleBlaster/BattleBlasterGameMode.h/.cpp` — level flow:

- `BeginPlay()` counts the towers, gives each of them a reference to the tank, shows
  the `ScreenMessage` widget and starts the countdown ("Get Ready!" → 3, 2, 1 → "Go!")
- `ActorDied(AActor*)` — handles a dead tank (defeat) or tower (when the last one is
  destroyed — victory) and shows "Victory!"/"Defeat!"
- after `GameOverDelay` loads the next level or restarts the current one through the
  game instance

`Source/BattleBlaster/BattleBlasterGameInstance.h/.cpp` — level progression that
survives level loads: `CurrentLevelIndex`/`LastLevelIndex`, `LoadNextLevel()`,
`RestartCurrentLevel()`, `RestartGame()`; levels are opened by name (`Level_<N>`).

`Source/BattleBlaster/ScreenMessage.h/.cpp` — a `UUserWidget` base with a bound
`MessageTextBlock` (`BindWidget`) and `SetMessageText()`.

## Content

- `Content/Blueprints/` — Blueprint wrappers on top of the C++ classes: `BP_Tank`,
  `BP_Tower`, `BP_Projectile`, `BP_GameMode`, `BP_GameInstance`,
  `BP_BattleBlasterPlayerController`, `WBP_ScreenMessage`, camera shakes
  (`BP_HitCameraShake`, `BP_DeathCameraShake`)
- `Content/Input/` — Enhanced Input assets: `IMC_Default`, `IA_Move`, `IA_Turn`,
  `IA_Fire`
- `Content/Maps/` — `Level_1`, `Level_2`, `Level_3` (default map: `Level_1`)
- `Content/Assets/` — meshes, materials, Niagara effects and sounds

## Controls

- Move / turn — `IA_Move` / `IA_Turn` (bound in `IMC_Default`)
- Aim — mouse cursor
- Fire — `IA_Fire`

## Tech notes

- Engine: UE 5.6 (engine installed at `D:\UE_5.6`)
- Module dependencies: `EnhancedInput`, `UMG`, `Niagara`
- Git LFS for binary assets (`.uasset`, `.umap`, textures, audio, `.fbx`)
- Generated folders (`Binaries/`, `Intermediate/`, `Saved/`, `DerivedDataCache/`) are
  gitignored

## Run

Open `BattleBlaster.uproject` in UE 5.6 (regenerate VS project files if needed).
