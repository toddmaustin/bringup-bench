/*
 * Copyright (C) 2022 Philip Jones
 *
 * Licensed under the MIT License.
 * https://opensource.org/licenses/MIT
 */
#include "libmin.h"

int32_t fuzzy_match(const char *restrict pattern, const char *restrict str);

static int32_t compute_score(
		int32_t jump,
		int first_char,
		const char *restrict match);

static int32_t fuzzy_match_recurse(
		const char *restrict pattern,
		const char *restrict str,
		int32_t score,
		int first_char);

/*
 * Returns score if each character in pattern is found sequentially within str.
 * Returns INT32_MIN otherwise.
 */
int32_t fuzzy_match(const char *restrict pattern, const char *restrict str)
{
	const int unmatched_letter_penalty = -1;
	const size_t slen = libmin_strlen(str);
	const size_t plen = libmin_strlen(pattern);
	int32_t score = 100;

	if (*pattern == '\0') {
		return score;
	}
	if (slen < plen) {
		return INT32_MIN;
	}

	/* We can already penalise any unused letters. */
	score += unmatched_letter_penalty * (int32_t)(slen - plen);

	/* Perform the match. */
	score = fuzzy_match_recurse(pattern, str, score, TRUE);

	return score;
}

/*
 * Recursively match the whole of pattern against str.
 * The score parameter is the score of the previously matched character.
 *
 * This reaches a maximum recursion depth of strlen(pattern) + 1. However, the
 * stack usage is small (the maximum I've seen on x86_64 is 144 bytes with
 * gcc -O3), so this shouldn't matter unless pattern contains thousands of
 * characters.
 */
int32_t fuzzy_match_recurse(
		const char *restrict pattern,
		const char *restrict str,
		int32_t score,
		int first_char)
{
	if (*pattern == '\0') {
		/* We've matched the full pattern. */
		return score;
	}

	const char *match = str;
	const char search[2] = { *pattern, '\0' };

	int32_t best_score = INT32_MIN;

	/*
	 * Find all occurrences of the next pattern character in str, and
	 * recurse on them.
	 */
	while ((match = libmin_strcasestr(match, search)) != NULL) {
		int32_t subscore = fuzzy_match_recurse(
				pattern + 1,
				match + 1,
				compute_score(match - str, first_char, match),
				FALSE);
		best_score = MAX(best_score, subscore);
		match++;
	}

	if (best_score == INT32_MIN) {
		/* We couldn't match the rest of the pattern. */
		return INT32_MIN;
	} else {
		return score + best_score;
	}
}

/*
 * Calculate the score for a single matching letter.
 * The scoring system is taken from fts_fuzzy_match v0.2.0 by Forrest Smith,
 * which is licensed to the public domain.
 *
 * The factors affecting score are:
 *   - Bonuses:
 *     - If there are multiple adjacent matches.
 *     - If a match occurs after a separator character.
 *     - If a match is uppercase, and the previous character is lowercase.
 *
 *   - Penalties:
 *     - If there are letters before the first match.
 *     - If there are superfluous characters in str (already accounted for).
 */
int32_t compute_score(int32_t jump, int first_char, const char *restrict match)
{
	const int adjacency_bonus = 15;
	const int separator_bonus = 30;
	const int camel_bonus = 30;
	const int first_letter_bonus = 15;

	const int leading_letter_penalty = -5;
	const int max_leading_letter_penalty = -15;

	int32_t score = 0;

	/* Apply bonuses. */
	if (!first_char && jump == 0) {
		score += adjacency_bonus;
	}
	if (!first_char || jump > 0) {
		if (isupper((unsigned char)*match)
				&& islower((unsigned char)*(match - 1))) {
			score += camel_bonus;
		}
		if (isalnum((unsigned char)*match)
				&& !isalnum((unsigned char)*(match - 1))) {
			score += separator_bonus;
		}
	}
	if (first_char && jump == 0) {
		/* Match at start of string gets separator bonus. */
		score += first_letter_bonus;
	}

	/* Apply penalties. */
	if (first_char) {
		score += MAX(leading_letter_penalty * jump,
				max_leading_letter_penalty);
	}

	return score;
}

const char *entries[] = {
  "Abomination",
  "Abusive Sergeant",
  "Acidic Swamp Ooze",
  "Acidmaw",
  "Acolyte of Pain",
  "Al'Akir the Windlord",
  "Alarm-o-Bot",
  "Aldor Peacekeeper",
  "Alexstrasza",
  "Alexstrasza's Champion",
  "Amani Berserker",
  "Ancestor's Call",
  "Ancestral Healing",
  "Ancestral Knowledge",
  "Ancestral Spirit",
  "Ancient Brewmaster",
  "Ancient Mage",
  "Ancient of Lore",
  "Ancient of War",
  "Ancient Shade",
  "Ancient Watcher",
  "Angry Chicken",
  "Anima Golem",
  "Animal Companion",
  "Animated Armor",
  "Annoy-o-Tron",
  "Anodized Robo Cub",
  "Antique Healbot",
  "Anub'ar Ambusher",
  "Anub'arak",
  "Anubisath Sentinel",
  "Anyfin Can Happen",
  "Arathi Weaponsmith",
  "Arcane Blast",
  "Arcane Explosion",
  "Arcane Golem",
  "Arcane Intellect",
  "Arcane Missiles",
  "Arcane Nullifier X-21",
  "Arcane Shot",
  "Arcanite Reaper",
  "Arch-Thief Rafaam",
  "Archmage",
  "Archmage Antonidas",
  "Argent Commander",
  "Argent Horserider",
  "Argent Lance",
  "Argent Protector",
  "Argent Squire",
  "Argent Watchman",
  "Armored Warhorse",
  "Armorsmith",
  "Assassin's Blade",
  "Assassinate",
  "Astral Communion",
  "Auchenai Soulpriest",
  "Avenge",
  "Avenging Wrath",
  "Aviana",
  "Axe Flinger",
  "Azure Drake",
  "Backstab",
  "Ball of Spiders",
  "Bane of Doom",
  "Baron Geddon",
  "Baron Rivendare",
  "Bash",
  "Battle Rage",
  "Bear Trap",
  "Beneath the Grounds",
  "Bestial Wrath",
  "Betrayal",
  "Big Game Hunter",
  "Bite",
  "Blackwing Corruptor",
  "Blackwing Technician",
  "Blade Flurry",
  "Blessed Champion",
  "Blessing of Kings",
  "Blessing of Might",
  "Blessing of Wisdom",
  "Blingtron 3000",
  "Blizzard",
  "Blood Imp",
  "Blood Knight",
  "Bloodfen Raptor",
  "Bloodlust",
  "Bloodmage Thalnos",
  "Bloodsail Corsair",
  "Bloodsail Raider",
  "Bluegill Warrior",
  "Bolf Ramshield",
  "Bolster",
  "Bolvar Fordragon",
  "Bomb Lobber",
  "Boneguard Lieutenant",
  "Booty Bay Bodyguard",
  "Boulderfist Ogre",
  "Bouncing Blade",
  "Brann Bronzebeard",
  "Brave Archer",
  "Brawl",
  "Buccaneer",
  "Burgle",
  "Burly Rockjaw Trogg",
  "Cabal Shadow Priest",
  "Cairne Bloodhoof",
  "Call Pet",
  "Captain Greenskin",
  "Captain's Parrot",
  "Captured Jormungar",
  "Cenarius",
  "Charge",
  "Charged Hammer",
  "Chillmaw",
  "Chillwind Yeti",
  "Chromaggus",
  "Circle of Healing",
  "Claw",
  "Cleave",
  "Clockwork Giant",
  "Clockwork Gnome",
  "Clockwork Knight",
  "Cobalt Guardian",
  "Cobra Shot",
  "Coghammer",
  "Cogmaster",
  "Cogmaster's Wrench",
  "Cold Blood",
  "Coldarra Drake",
  "Coldlight Oracle",
  "Coldlight Seer",
  "Coliseum Manager",
  "Commanding Shout",
  "Competitive Spirit",
  "Conceal",
  "Cone of Cold",
  "Confessor Paletress",
  "Confuse",
  "Consecration",
  "Convert",
  "Core Hound",
  "Core Rager",
  "Corruption",
  "Counterspell",
  "Crackle",
  "Crazed Alchemist",
  "Crowd Favorite",
  "Cruel Taskmaster",
  "Crush",
  "Cult Master",
  "Curse of Rafaam",
  "Cursed Blade",
  "Cutpurse",
  NULL
};

int
main(void)
{
  {
	  const char *pattern = "core";
    libmin_printf("Matches for `%s':\n", pattern);
    for (int i=0; entries[i] != NULL; i++)
    {
		  int32_t score = fuzzy_match(pattern, entries[i]);
		  if (score != INT32_MIN) {
			  libmin_printf("  %3d|%s\n", score, entries[i]);
		  }
	  }
    libmin_printf("\n");
  }

  {
	  const char *pattern = "work";
    libmin_printf("Matches for `%s':\n", pattern);
    for (int i=0; entries[i] != NULL; i++)
    {
		  int32_t score = fuzzy_match(pattern, entries[i]);
		  if (score != INT32_MIN) {
			  libmin_printf("  %3d|%s\n", score, entries[i]);
		  }
	  }
    libmin_printf("\n");
  }

  {
	  const char *pattern = "sam";
    libmin_printf("Matches for `%s':\n", pattern);
    for (int i=0; entries[i] != NULL; i++)
    {
		  int32_t score = fuzzy_match(pattern, entries[i]);
		  if (score != INT32_MIN) {
			  libmin_printf("  %3d|%s\n", score, entries[i]);
		  }
	  }
    libmin_printf("\n");
  }

  libmin_success();	
  return 0;
}

