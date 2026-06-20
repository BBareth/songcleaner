# Song Cleaner

Every saved online level keeps its custom song as a downloaded `.mp3` in your `GeometryDash` AppData folder. Those files stick around forever, even after you remove the level — so a few hundred deleted levels later, your save folder is bloated with songs you'll never hear again.

**Song Cleaner fixes that.** When you delete a saved level, the mod automatically deletes the song file too, freeing up disk space.

## What it does

- Hooks into the game's level-deletion path.
- Looks at the level's custom song ID (plus any extra song IDs from 2.2 multi-song levels).
- Calls the game's own song-removal API to clean up the `.mp3` files on disk.

## Safe by design

- **Built-in tracks are never touched.** Stereo Madness, Back On Track and friends use the game's bundled audio — the mod only ever considers songs in your downloaded-songs folder.
- **Shared songs are protected.** Before deleting, the mod scans every other saved online level *and* every local editor level. If anything still references the song, it stays.
- **Opt-out anytime.** Both behaviors are toggleable in the mod's settings.

## Settings

- **Enable Auto-Cleanup** — master switch.
- **Skip songs used by other levels** — keep the safety check on (recommended). Turn it off only if you want unconditional deletion.

## Why?

Custom songs are easy to re-download, but there's no built-in "clean unused songs" button in Geometry Dash. This mod just folds that cleanup into the action you already perform when you're done with a level.
