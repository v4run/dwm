/* See LICENSE file for copyright and license details. */

#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int gappx     = 15;       /* gaps between windows */
static const unsigned int snap      = 1;        /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */

static const char *fonts[]    = { "monospace:size=10", "FantasqueSansMono Nerd Font Mono:style=Regular:size=15" };
static const char dmenufont[] = "monospace:size=11";

static const char col_norm_bg[]      = "#040404";
static const char col_norm_border[]  = "#161313";
static const char col_sel_border[]   = "#615756";
static const char col_norm_fg[]      = "#dce2c8";
static const char col_sel_bg[]       = "#040404";
static const char col_sel_fg[]       = "#ec4e20";
static const char col_dmenu_sel_bg[] = "#ec4e20";
static const char col_dmenu_sel_fg[] = "#040404";
static const unsigned int baralpha = 0xd0;
static const unsigned int borderalphanorm = OPAQUE;
static const unsigned int borderalphasel = OPAQUE;
static const char *colors[][3]      = {
	/*                      fg                       bg                 border   */
	[SchemeNorm]     = { col_norm_fg,          col_norm_bg,          col_norm_border },
	[SchemeSel]      = { col_sel_fg,           col_sel_bg,           col_sel_border  },
};
static const unsigned int alphas[][3]      = {
	/*               fg      bg        border     */
	[SchemeNorm] = { OPAQUE, baralpha, borderalphanorm },
	[SchemeSel]  = { OPAQUE, baralpha, borderalphasel },
};

/* tagging */
static const char *tags[] = { "", "", "", "", "" };

static const unsigned int ulinepad	= 5;	/* horizontal padding between the underline and tag */
static const unsigned int ulinestroke	= 1;	/* thickness / height of the underline */
static const unsigned int ulinevoffset	= 0;	/* how far above the bottom of the bar the line should appear */
static const int ulineall 		= 0;	/* 1 to show underline on all tags, 0 for just the active ones */

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Chromium",  NULL,       NULL,       1 << 1,       0,           -1 },
	{ "Signal",    NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Slack",     NULL,       NULL,       1 << 2,       0,           -1 },
	{ "Music",     NULL,       NULL,       1 << 3,       0,           -1 },
	{ "helvum",    NULL,       NULL,            0,       1,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[﩯]",      tile },    /* first entry is default */
	{ "[]",      NULL },    /* no layout function means floating behavior */
	{ "[]",      monocle },
};

/* Volume buttons */
static const char *upvol[]   = { "sh", "-c", "/usr/bin/pamixer --increase 2 && kill -36 $(pidof goblocks)", NULL };
static const char *downvol[] = { "sh", "-c", "/usr/bin/pamixer --decrease 2 && kill -36 $(pidof goblocks)", NULL };
static const char *mutevol[] = { "sh", "-c", "/usr/bin/pamixer --toggle-mute && kill -36 $(pidof goblocks)", NULL };

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/usr/local/bin/st", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-i", "-m", dmenumon, "-fn", dmenufont, "-nb", col_norm_bg, "-nf", col_norm_fg, "-sb", col_dmenu_sel_bg, "-sf", col_dmenu_sel_fg, NULL };
static const char *termcmd[]  = { "st", "-e", "tmux", NULL };
static const char *browsercmd[]  = { "chromium", NULL };
static const char *musiccmd[]  = { "st", "-t", "ncmpcpp", "-c", "Music", "-e", "ncmpcpp", NULL };
static const char *screenshotcmd[]  = { "scrot", "-s", "-e", "xclip -selection clipboard -t image/png -i $f", NULL };

static Key keys[] = {
	/* modifier                     key                        function        argument */
	{ 0,                            XF86XK_AudioLowerVolume,   spawn,          {.v = downvol } },
	{ 0,                            XF86XK_AudioMute,          spawn,          {.v = mutevol } },
	{ 0,                            XF86XK_AudioRaiseVolume,   spawn,          {.v = upvol   } },
	{ MODKEY,                       XK_p,                      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,                 spawn,          {.v = termcmd } },
	{ MODKEY|ShiftMask,             XK_b,                      spawn,          {.v = browsercmd } },
	{ MODKEY|ShiftMask,             XK_m,                      spawn,          {.v = musiccmd } },
	{ MODKEY|ShiftMask,             XK_s,                      spawn,          {.v = screenshotcmd } },
	{ MODKEY,                       XK_b,                      togglebar,      {0} },
	{ MODKEY,                       XK_j,                      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,                      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,                      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,                      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,                      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,                      setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,                 zoom,           {0} },
	{ MODKEY,                       XK_Tab,                    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,                      killclient,     {0} },
	{ MODKEY,                       XK_t,                      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_space,                  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,                  togglefloating, {0} },
	{ MODKEY,                       XK_0,                      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,                  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period,                 focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,                  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,                 tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,                  setgaps,        {.i = -1 } },
	{ MODKEY,                       XK_equal,                  setgaps,        {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_equal,                  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                                      0)
	TAGKEYS(                        XK_2,                                      1)
	TAGKEYS(                        XK_3,                                      2)
	TAGKEYS(                        XK_4,                                      3)
	TAGKEYS(                        XK_5,                                      4)
	/* TAGKEYS(                        XK_6,                                      5) */
	/* TAGKEYS(                        XK_7,                                      6) */
	/* TAGKEYS(                        XK_8,                                      7) */
	/* TAGKEYS(                        XK_9,                                      8) */
	{ MODKEY|ShiftMask,             XK_q,                      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

