/* See LICENSE file for copyright and license details. */
// vim: set noet: ts=8 sts=4 sw=4

/* appearance */
static const char *fonts[] = {
    "Sans:size=10.5",
    "VL Gothic:size=10.5",
    "WenQuanYi Micro Hei:size=10.5",
};
static const char dmenufont[] = "-*-terminus-medium-r-*-*-16-*-*-*-*-*-*-*";
static const char normbordercolor[] = "#444444";
static const char normbgcolor[]     = "#222222";
static const char normfgcolor[]     = "#bbbbbb";
static const char selbordercolor[]  = "#005577";
static const char selbgcolor[]      = "#005577";
static const char selfgcolor[]      = "#eeeeee";
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const Bool showbar           = True;     /* False means no bar */
static const Bool topbar            = True;     /* False means bottom bar */

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance             title       tags mask     isfloating   monitor */
	{ "Gimp",           NULL,                NULL,       0,            True,        -1 },
	{ "Firefox",        NULL,                NULL,       1 << 2,       False,       -1 },
	{ "Google-chrome",  NULL,                NULL,       1 << 4,       False,       -1 },
	{ "Mail",           NULL,                NULL,       1 << 3,       False,       -1 },
	{ "Gnome-terminal", "WeeChat 1.2",       NULL,       1,            False,       -1 },
};

/* layout(s) */
static const float mfact      = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster      = 1;    /* number of clients in master area */
static const Bool resizehints = True; /* True means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "[▣]",      centeredmaster },
};

/* key definitions */
#define MODKEY Mod4Mask         // Mod4Mask = SUPER (WINDOWS) key
#define ALT_MODKEY Mod1Mask     // Mod1Mask = ALT key
#define XK_vol_up   0x1008ff13  // Volume up key
#define XK_vol_down 0x1008ff11  // Volume down key
#define XK_tog_mute 0x1008ff12  // Toggle mute key
#define XK_brg_up   0x1008ff02  // Brightness up
#define XK_brg_down 0x1008ff03  // Brightness down
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[]   = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbgcolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]    = { "gnome-terminal", "--hide-menubar", NULL };
static const char *lockcmd[]    = { "slock", NULL };
static const char *cmdvolup[]   = { "amixer", "-q", "sset", "Master", "5%+", NULL };
static const char *cmdvoldown[] = { "amixer", "-q", "sset", "Master", "5%-", NULL };
static const char *cmdsndmute[] = { "amixer", "-q", "sset", "Master", "toggle", NULL };
static const char *cmdbrgup[]   = { "xbacklight", "-inc", "10", NULL };
static const char *cmdbrgdown[] = { "xbacklight", "-dec", "10", NULL };
static const char *doercmd[]    = { "doer", NULL };

static Key keys[] = {
	/* modifier                     key             function        argument */
	{ ALT_MODKEY,                   XK_F2,          spawn,          {.v = dmenucmd }},
	{ ALT_MODKEY|ControlMask,       XK_l,           spawn,          {.v = lockcmd }},
	{ MODKEY|ControlMask,           XK_Return,      spawn,          {.v = termcmd }},
    { ALT_MODKEY,                   XK_d,           spawn,          {.v = doercmd }},

    /* Sound */
    { 0,                            XK_vol_up,      spawn,          {.v = cmdvolup }},
    { 0,                            XK_vol_down,    spawn,          {.v = cmdvolup }},
    { 0,                            XK_tog_mute,    spawn,          {.v = cmdsndmute }},

    /* Brightness */
    { 0,                            XK_brg_up,      spawn,          {.v = cmdbrgup }},
    { 0,                            XK_brg_down,    spawn,          {.v = cmdbrgdown }},

    /* Terminating dwm */
    { MODKEY,                       XK_q,           quit,            {.ui = EXIT_SUCCESS}},
    { MODKEY,                       XK_r,           quit,            {.ui = 129}},

    /* Layouts */
	{ MODKEY,                       XK_t,           setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,           setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,           setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_s,           setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_d,           setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_c,           setlayout,      {.v = &layouts[5]} },
	{ MODKEY,                       XK_space,       setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,       togglefloating, {0} },

    /* Window focus */
	{ ALT_MODKEY,                   XK_Tab,         focusstack,     {.i = +1 } },
	{ ALT_MODKEY|ShiftMask,         XK_Tab,         focusstack,     {.i = -1 } },


	{ MODKEY,                       XK_b,           togglebar,      {0} },
	{ MODKEY,                       XK_plus,        incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,       incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_KP_Add,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_KP_Subtract, incnmaster,     {.i = -1 } },

	{ MODKEY|ShiftMask,             XK_Left,        setmfact,       {.f = -0.05} },
	{ MODKEY|ShiftMask,             XK_Right,       setmfact,       {.f = +0.05} },
	{ MODKEY,                       XK_Return,      zoom,           {0} },

	{ MODKEY,                       XK_Left,        focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_Right,       focusmon,       {.i = +1 } },
	{ MODKEY|ControlMask,           XK_Left,        tagmon,         {.i = -1 } },
	{ MODKEY|ControlMask,           XK_Right,       tagmon,         {.i = +1 } },


	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
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

