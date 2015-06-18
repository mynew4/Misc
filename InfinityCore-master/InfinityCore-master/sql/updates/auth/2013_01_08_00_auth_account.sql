ALTER TABLE `account`
    ADD COLUMN `last_build` tinyint(3) unsigned NOT NULL DEFAULT '0' AFTER `recruiter`;
