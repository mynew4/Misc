-- Brain Freeze
DELETE FROM `spell_proc_event` WHERE `entry` IN (44546);
INSERT INTO `spell_proc_event` VALUES (44546, 0x10, 0x03, 0x00000020, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0, 5, 0);
DELETE FROM `spell_proc_event` WHERE `entry` IN (44548);
INSERT INTO `spell_proc_event` VALUES (44548, 0x10, 0x03, 0x00000020, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0, 10, 0);
DELETE FROM `spell_proc_event` WHERE `entry` IN (44549);
INSERT INTO `spell_proc_event` VALUES (44549, 0x10, 0x03, 0x00000020, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0, 15, 0);

-- Fingers of Frost
DELETE FROM `spell_proc_event` WHERE `entry` IN (44543);
INSERT INTO `spell_proc_event` VALUES (44543, 0x10, 0x03, 0x00000060, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0, 7, 0);
DELETE FROM `spell_proc_event` WHERE `entry` IN (44545);
INSERT INTO `spell_proc_event` VALUES (44545, 0x10, 0x03, 0x00000060, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0, 14, 0);
DELETE FROM `spell_proc_event` WHERE `entry` IN (83074);
INSERT INTO `spell_proc_event` VALUES (83074, 0x10, 0x03, 0x00000060, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0, 20, 0);