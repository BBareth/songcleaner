# Changelog

## v1.0.0

- Initial release.
- Hook `GameLevelManager::deleteLevel` to delete the level's custom song mp3 when the level is removed.
- Skip built-in audio tracks and songs that are still referenced by another saved or local level.
- Settings: master toggle, and an option to force-delete even shared songs.
