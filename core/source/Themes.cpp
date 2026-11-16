#include "Themes.h"
#include <stddef.h>

// COLORS
static constexpr uint32_t one_true_red = 0xcd1818;
static constexpr uint32_t one_true_green = 0x30cb3f;
static constexpr uint32_t white = 0xFFFFFF;
static constexpr uint32_t black = 0x000000;
static constexpr uint32_t cyan = 0x3FFFFF;
static constexpr uint32_t dark_cyan = 0x00A4B3;
static constexpr uint32_t red = 0xFF0000;
static constexpr uint32_t green = 0x00FF00;
static constexpr uint32_t orange = 0xD67022;

static constexpr uint32_t instrYellow = 0xfef749;

static constexpr uint32_t grey_08 = 0x080808;
static constexpr uint32_t grey_0A = 0x0A0A0A;
static constexpr uint32_t grey_12 = 0x121212;
static constexpr uint32_t grey_22 = 0x222222;
static constexpr uint32_t grey_32 = 0x323132;
static constexpr uint32_t grey_33 = 0x333333;
static constexpr uint32_t grey_37 = 0x373737;
static constexpr uint32_t grey_3F = 0x3F3F3F;
static constexpr uint32_t grey_44 = 0x444344;
static constexpr uint32_t grey_57 = 0x575757;
static constexpr uint32_t grey_77 = 0x777777;
static constexpr uint32_t grey_90 = 0x909090;

static constexpr uint32_t grey_X08 = 0xC7C7C7;
static constexpr uint32_t grey_X0A = 0xC0C0C0;
static constexpr uint32_t grey_X12 = 0xA8A8A8;
static constexpr uint32_t grey_X22 = 0x7E7E7E;
static constexpr uint32_t grey_X32 = 0x6E6E6E;
static constexpr uint32_t grey_X33 = 0x6D6D6D;
static constexpr uint32_t grey_X37 = 0x696969;
static constexpr uint32_t grey_X3F = 0x616161;
static constexpr uint32_t grey_X44 = 0x5C5C5C;
static constexpr uint32_t grey_X57 = 0x494949;
static constexpr uint32_t grey_X77 = 0x292929;

// clang-format off

constexpr Theme themeDefault =
{
	.backgroundColor = black,
	//------------------------------
                                        // tekst         tło           ramka         tekst2
	.topBarLabelColors =                 { white,        grey_08,      white,        white},
	.songTimerLabelColors =              { white,        grey_08,      one_true_red, white},

    .activeLabelsColors =                { grey_77,      grey_32,      one_true_red, white},
    .inactiveLabelsColors =              { grey_33,      grey_32,      one_true_red, grey_57},
    .activeButtonLabelsColors =          { grey_77,      grey_32,      one_true_red, grey_77},
    .inactiveButtonLabelsColors =        { grey_33,      grey_32,      one_true_red, grey_33},
	.highlightedButtonLabelsColors =     { grey_77,      grey_22,      one_true_red, grey_77},
	.redButtonLabelsColors =             { grey_77,      one_true_red, one_true_red, grey_77},
    .masterEditTrackNameLabelsColors =   { grey_77,      grey_32,      one_true_red, one_true_red},
    .redBlinkLabelColors =               { one_true_red, one_true_red, one_true_red, one_true_red},
    .performActiveTrackLabelsColors =    { grey_77,      one_true_red, one_true_red, white},
    .performMutedTrackLabelsColors =     { black,        one_true_red, one_true_red, black},
	.blankSpaceCoverLabelColors =        { white,        grey_0A,      red,          black},


	.patchEditSynthModelIconColors =     { white,        grey_0A,      red,     black},


	.bottomButtonsBgColors =             { grey_77,      grey_0A,      one_true_red},

	                                    // text          box-bg        box-sel       bg         frame          dots-bg   dots-sel
	.miniBoxMenuColors =                 { white,        grey_22,      grey_32,      black,     one_true_red,  grey_12,  grey_77},

	                                    // text          active img    non-active img  active square  non-active sqr   frame
	.imgGridColors =                     { white,        grey_32,      white,          white,         grey_32,         one_true_red, },

                                        // kolor główny, kolor dod. 1, kolor dod. 2, kontener, tło
	.activeBarColors =                   { white,        red,          green,        grey_08,  grey_0A},
	.inactiveBarColors =                 { grey_22,      red,          green,        grey_08,  grey_0A},

                                         // frameNormal, frameSelect,  listScrollBar,  listBg,   fontNormal, fontSelection,  scrollParCont
	.activeListColors =                  { white,        white,        grey_57,        grey_0A,  white,      white,          black},
	.inactiveListColors =                { grey_22,      white,        grey_57,        grey_0A,  grey_22,    grey_22,        black},
	.projectActiveListColors =           { one_true_red, one_true_red, grey_57,        grey_0A,  white,      one_true_green, black},
	.projectInactiveListColors =         { white,        white,        grey_57,        grey_0A,  white,      one_true_green, black},
    .sampleLoaderActiveListColors =      { one_true_red, one_true_red, grey_57,        grey_0A,  white,      white,          black},
    .sampleLoaderInactiveListColors =    { white,        white,        grey_57,        grey_0A,  white,      white,          black},

	                                    // tekst         kontener      ramka          zaznaczenie  tło
	.editBoxColors =                     { white,        grey_08,      one_true_red,  grey_22,     grey_0A},

    .frameColors =                       { one_true_red,   grey_3F},
    .greenFrameColors =                  { one_true_green, grey_3F},

                                        // klawisze      tekst         ramka
	.keyboardColors =                    { grey_0A,      white,        one_true_red},

	//------------------------------

    .stepPopupTitleLabelColor =          { black,        white,        one_true_red, black},
    .stepPopupListColor =                { white,        black,        grey_57,      grey_0A,  white,    white,        black}, // activeListColors
    .stepPopupBgLabelColor =             { white,        grey_0A,      red , 		 black},
    .stepPopupDescriptionTextBoxColors = { white,        grey_0A,      one_true_red}, // valueLabelColorsEdit

    .notePopupColors =                   { grey_0A,      white,        one_true_red},

										 // text         bg            text 2
	.noteKeyboardColors =                { grey_77,      grey_08,      white},

    .popupHorizontalBarColors =          { grey_22,      grey_22,      white,        grey_12},
                                        // text          bg            shadow
	.popupTextColors =                   { white,        grey_22,      black},

	                                    // tlo           ramka         pasek/tekst
	.popupWithProgressColors =           { grey_22,      grey_22,      white},

	.noSdInfoTextColors =                { white,        white},

	//------------------------------
                                        // 0                1         2         3            4         5         6             7              8
                                        // nazwy tr,        backgr,   nuta,     instr,       FX 1,     FX 2,     nuta nieakt,  instr nieakt,  FX 1 nieakt,
	.patternTrackerColors =              { black,           grey_08,  0x00e8be, instrYellow, 0xbb58f1, 0x57f1ff, 0x003933,     0x453b1b,      0x321f3a,
			                            // 9                10             11         12            13         14         15              16
			                            // FX 2 nieakt,     zaznaczenie,   podziałka, track nieakt, playhead,  txt zazn., podziałka pion, step num
			                               0x183744,        one_true_red,  0x0e0e0e,  0x171717,     black,     white,     black,          grey_57,
                                        // 17               18         19              20
								        // green mode,      unfocused  track aktywnny  trackend
								           one_true_green,  grey_44,   grey_57,        instrYellow},

	.patternFillBgLabelColors =          { white,        black,    red,   white},

                                        // texty        activ bar/block  scroll     tlo        noact text    editmode        scroll bg   playhead/mute     emptytrack   playhead/activ bar
	.songPlayerColors =                  { white,       white,           grey_57,   grey_0A,   grey_33,      one_true_red,   black,      grey_44,          grey_12,     one_true_green},


	.configItemEditorTextLabelColors =   { white,        grey_0A,      red,          black },
    .gridEditorColors =                  { grey_0A,      white,        one_true_red },
    .configConfirmPopUpLabelColors =     { white,        grey_22,      one_true_red, white},


    .sampleLoadMemBarNormalColors =      { white,        one_true_red, one_true_green, grey_08,  grey_0A},
    .sampleLoadMemBarFullColors =        { one_true_red, one_true_red, one_true_red,   grey_08,  grey_0A},


    .perfFxNameTextLabelColors =         { white,        grey_12,      red,             white},
	.perfValueNormalMLabelColors =       { white,        grey_0A,      one_true_green  },
	.perfValueEditMLabelColors =         { white,        grey_0A,      one_true_red    },


	.startScreenColors =                 { white,        white},

	//------------------------------

    .waveformColors =                    { white,        grey_77},
    .waveformPointsColors =              { white,        grey_0A,      one_true_red,    one_true_green},
    .waveformProgressColors =            { white,        one_true_red},
    .granularProgressColors =            { cyan,         grey_22},
    .waveTablePositionColors =           { white,        white},
    .sliceColors =                       { instrYellow,     one_true_red, one_true_green },

	//------------------------------

	.batteryIconColors =                 { grey_90, green, red},

	.defaultTextColors =                 { black,        white,        one_true_red, black},


    // bars colors
    .white = white,
    .red = one_true_red,
	.orange = orange,
	.yellow = instrYellow,
	.green = one_true_green,
    .cyan = cyan,
    .grey_22 = grey_22,

    .bottomScreenShadow = 1,
    .barShadow = 1,
    .patternEditorPlayheadShadow = 1,
    .patternEditorBottomButtonsLines = 1,
    .moreButtonColors =                  { grey_77,      grey_22,      grey_77,      grey_22},
};

// clang-format on

static constexpr uint32_t grey_71 = 0x717171;
static constexpr uint32_t grey_C9CA = 0xC9C9CA;

static constexpr uint32_t background = 0x4B4B4B;

static constexpr uint32_t block = 0x3E3E3E;
static constexpr uint32_t dark_block = 0x303030;

static constexpr uint32_t active_object = 0xC9C9CA;
static constexpr uint32_t inactive_object = 0x303030;

static constexpr uint32_t bright = 0xB9B9BA;

static constexpr uint32_t bright_text = 0xC9C9CA;
static constexpr uint32_t active_text = white;
static constexpr uint32_t inactive_text = 0x303030;
static constexpr uint32_t two_true_red = 0xdd2020;

// clang-format off

constexpr Theme themeWhiteout =
{
	.backgroundColor = background,
	//------------------------------
                                        // tekst          tło            ramka             tekst2
	.topBarLabelColors =                 { black,         bright,        white,            white},
	.songTimerLabelColors =              { black,         bright,        two_true_red,     white},

    .activeLabelsColors =                { active_text,   grey_X32,      two_true_red,     active_text},
    .inactiveLabelsColors =              { inactive_text, grey_X32,      two_true_red,     inactive_text},
    .activeButtonLabelsColors =          { active_text,   grey_X32,      two_true_red,     active_text},
    .inactiveButtonLabelsColors =        { inactive_text, grey_X32,      two_true_red,     inactive_text},
	.highlightedButtonLabelsColors =     { active_text,   grey_X77,      two_true_red,     active_text},
	.redButtonLabelsColors =             { active_text,   two_true_red,  two_true_red,     active_text},
    .masterEditTrackNameLabelsColors =   { active_text,   grey_X32,      two_true_red,     two_true_red},
    .redBlinkLabelColors =               { two_true_red,  two_true_red,  two_true_red,     two_true_red},
    .performActiveTrackLabelsColors =    { active_text,   two_true_red,  two_true_red,     active_text},
    .performMutedTrackLabelsColors =     { active_text,   0xC64E4D,      two_true_red,     active_text},

    .blankSpaceCoverLabelColors =        { active_text,   block,         two_true_red,     active_text},

	.patchEditSynthModelIconColors =     { white,        grey_0A,      red,     black},

                                        // linie         tło            ramka
	.bottomButtonsBgColors =             { grey_71,      background,    two_true_red},

                                        // text          box1-bg       box2-sel     bg       frame           dots-bg     dots-sel
    .miniBoxMenuColors =                 { grey_22,      grey_X22,     grey_X44,    black,   one_true_red,   grey_X77,   grey_X12},

                                        // text          active img    non-active img  active square  non-active sqr   frame
    .imgGridColors =                     { white,        grey_32,      white,          white,         grey_32,         one_true_red, },

                                        // kolor główny,       kolor dod. 1,      kolor dod. 2,          kontener,  tło
	.activeBarColors =                   { active_object,      two_true_red,      one_true_green,        block,     block},
	.inactiveBarColors =                 { inactive_object,    two_true_red,      one_true_green,        block,     block},

	                                     // frameNormal,  frameSelection, listScrollBar,  listBg,   fontNormal,  fontSelection,  scrollParCont
	.activeListColors =                  { active_object, active_object,  grey_X77,       block,    bright_text, bright_text,    grey_X22},
	.inactiveListColors =                { grey_X22,      active_object,  grey_X77,       block,  	0x303030,    0x303030,       grey_X22},
	.projectActiveListColors =           { active_object, two_true_red,   grey_X77,       block,  	bright_text, one_true_green, grey_X22},
	.projectInactiveListColors =         { active_object, active_object,  grey_X77,       block,  	bright_text, one_true_green, grey_X22},
    .sampleLoaderActiveListColors =      { active_object, two_true_red,   grey_X77,       block,  	bright_text, bright_text,    grey_X22},
    .sampleLoaderInactiveListColors =    { active_object, active_object,  grey_X77,       block,  	bright_text, bright_text,    grey_X22},

	                                    // tekst          kontener   ramka             zaznaczenie        tło
	.editBoxColors =                     { active_text,   block,     two_true_red,     background,        background},

    .frameColors =                       { two_true_red,           grey_X3F},
    .greenFrameColors =                  { one_true_green,         grey_X3F},

                                        // klawisze       tekst              ramka
	.keyboardColors =                    { block,         active_text,     two_true_red},

	//------------------------------

    .stepPopupTitleLabelColor =          { active_text,   background,   two_true_red,    black},
                                        // listItemFr,    listItemFrBg, listScrollBar,   listBg,    fontList,        select,        scrollParCont
    .stepPopupListColor =                { active_object, background,   grey_X77,        block,     active_text,     active_text,   grey_X22},
    .stepPopupBgLabelColor =             { active_text,   block,        two_true_red,    black},
    .stepPopupDescriptionTextBoxColors = { active_text,   block,        two_true_red},

    .notePopupColors =                   { block,         active_text,  two_true_red},

	 	 	 	 	 	 	 	 	 	 // text         bg             text 2
    .noteKeyboardColors =                { grey_C9CA,   background,     active_text},

    .popupHorizontalBarColors =          { grey_X22,      background,   active_object,   block},
    .popupTextColors =                   { active_text,   background,   two_true_red},

                                        // tlo            ramka          pasek/tekst
	.popupWithProgressColors =           { grey_X22,      background,      active_text},

	.noSdInfoTextColors =                { active_text, active_text},

	//------------------------------
                                        // 0            1           2            3              4             5              6               7               8
                                        // nazwy tr,    backgr,     nuta,        instr,         FX 1,         FX 2,          nuta nieakt,    instr nieakt,   FX 1 nieakt,
	.patternTrackerColors =              { active_text, block,      active_text, active_text,   active_text,  active_text,   active_text,    active_text,    active_text,
			                            // 9            10             11          12             13                14         15              16
			                            // FX 2 nieakt, zaznaczenie,   podziałka,  track nieakt,  playhead,         txt zazn., podziałka pion  step num
			                               active_text, two_true_red,  0x454545,   inactive_text, inactive_object,  black,     grey_71,        active_text,
	                                    // 17               18         19                  20
								        // green mode       white mode     track aktywnny  trackend
								           one_true_green,  active_text,   dark_block,     instrYellow },

	.patternFillBgLabelColors =          { black,        background,        two_true_red,    black},

	                                    // texty         activ bar/block  scroll      tlo         noact text       editmode        scroll bg   playhead/mute      emptytrack  playhead/activ bar
	.songPlayerColors =                  { active_text,  active_object,   grey_X77,   background, inactive_object, two_true_red,   grey_X22,   inactive_object,   block,      one_true_green},


	.configItemEditorTextLabelColors =   { active_text,  grey_X0A,      two_true_red,      active_text},
	.gridEditorColors =                  { grey_X0A,     black,         two_true_red},
    .configConfirmPopUpLabelColors =     { active_text,  grey_X22,      two_true_red,      black},

                                        // kolor główny,    kolor dod. 1,    kolor dod. 2,    kontener,    tło
    .sampleLoadMemBarNormalColors =      { active_object,   two_true_red,    one_true_green,  block,       block},
    .sampleLoadMemBarFullColors =        { two_true_red,    two_true_red,    two_true_red,    block,       block},


    .perfFxNameTextLabelColors =         { bright_text,      grey_X12,      two_true_red,        black},
    .perfValueNormalMLabelColors =       { bright_text,      block,         one_true_green       },
    .perfValueEditMLabelColors =         { bright_text,      block,         two_true_red         },

	.startScreenColors =                 { active_text,      active_object},

	//------------------------------

    .waveformColors =                    { active_object,    grey_77},
    .waveformPointsColors =              { white,            grey_0A,       two_true_red,        one_true_green},
    .waveformProgressColors =            { black,            two_true_red},
    .granularProgressColors =            { cyan,             grey_X22},
    .waveTablePositionColors =           { black,            black},
    .sliceColors =                       { instrYellow,         two_true_red,  one_true_green  },

	//------------------------------

	.batteryIconColors =                 {grey_90, green, red},

	.defaultTextColors =                 { active_text,        black,         two_true_red,         active_text},


    // bars colors
    .white = active_object,
    .red = two_true_red,
	.orange = orange,
	.yellow = instrYellow,
	.green = one_true_green,
    .cyan = dark_cyan,
    .grey_22 = inactive_object,

    .bottomScreenShadow = 0,
    .barShadow = 0,
    .patternEditorPlayheadShadow = 1,
    .patternEditorBottomButtonsLines = 1,
    .moreButtonColors =                  { active_text,   grey_X32,      active_text,      grey_X32},
};

// clang-format on

static constexpr uint32_t patt_text = 0xD0D0D0;
static constexpr uint32_t patt_text2 = 0x454545;

// clang-format off

constexpr Theme themeAnniversary=
{
	.backgroundColor = black,
	//------------------------------
                                        // tekst          tło            ramka             tekst2
	.topBarLabelColors =                 { white,         black,         white,            white},
	.songTimerLabelColors =              { white,         black,         two_true_red,     white},

    .activeLabelsColors =                { active_text,   grey_X32,      two_true_red,     active_text},
    .inactiveLabelsColors =              { inactive_text, grey_X32,      two_true_red,     inactive_text},
    .activeButtonLabelsColors =          { active_text,   grey_X32,      two_true_red,     active_text},
    .inactiveButtonLabelsColors =        { inactive_text, grey_X32,      two_true_red,     inactive_text},
	.highlightedButtonLabelsColors =     { active_text,   grey_X77,      two_true_red,     active_text},
	.redButtonLabelsColors =             { active_text,   two_true_red,  two_true_red,     active_text},
    .masterEditTrackNameLabelsColors =   { active_text,   grey_X32,      two_true_red,     two_true_red},
    .redBlinkLabelColors =               { two_true_red,  two_true_red,  two_true_red,     two_true_red},
    .performActiveTrackLabelsColors =    { active_text,   two_true_red,  two_true_red,     active_text},
    .performMutedTrackLabelsColors =     { active_text,   0xC64E4D,      two_true_red,     active_text},

    .blankSpaceCoverLabelColors =        { active_text,   black,         two_true_red,     active_text},

	.patchEditSynthModelIconColors =     { white,        grey_0A,      red,     black},

                                        // linie         tło            ramka
	.bottomButtonsBgColors =             { black,        black,         two_true_red},

                                        // text          box-bg        box-sel       bg         frame          dots-bg   dots-sel
    .miniBoxMenuColors =                 { white,        grey_22,      grey_32,      black,     one_true_red,  grey_12,  grey_77},

                                        // text          active img    non-active img  active square  non-active sqr   frame
    .imgGridColors =                     { white,        grey_32,      white,          white,         grey_32,         one_true_red, },

                                        // kolor główny,       kolor dod. 1,      kolor dod. 2,          kontener,  tło
	.activeBarColors =                   { active_object,      two_true_red,      one_true_green,        black,     black},
	.inactiveBarColors =                 { inactive_object,    two_true_red,      one_true_green,        black,     black},

	                                     // frameNormal,  frameSelection, listScrollBar, listBg,   fontNormal,   fontSelection,  scrollParCont
	.activeListColors =                  { active_object, active_object,  grey_57,       black,    bright_text,  bright_text,    black},
	.inactiveListColors =                { grey_X22,      active_object,  grey_57,       black,  	0x303030,    0x303030,       black},
	.projectActiveListColors =           { active_object, two_true_red,   grey_57,       black,  	bright_text, one_true_green, black},
	.projectInactiveListColors =         { active_object, active_object,  grey_57,       black,  	bright_text, one_true_green, black},
    .sampleLoaderActiveListColors =      { active_object, two_true_red,   grey_57,       black,  	bright_text, bright_text,    black},
    .sampleLoaderInactiveListColors =    { active_object, active_object,  grey_57,       black,  	bright_text, bright_text,    black},

	                                    // tekst          kontener   ramka             zaznaczenie        tło
	.editBoxColors =                     { active_text,   black,     two_true_red,     background,        background},

    .frameColors =                       { two_true_red,           grey_X3F},
    .greenFrameColors =                  { one_true_green,         grey_X3F},

                                        // klawisze       tekst              ramka
	.keyboardColors =                    { block,         active_text,     two_true_red},

	//------------------------------

    .stepPopupTitleLabelColor =          { active_text,   black,   two_true_red,         black},
                                        // listItemFr,    listItemFrBg, listScrollBar,   listBg,    fontList,        select,        scrollParCont
    .stepPopupListColor =                { active_object, black,        grey_57,         black,     active_text,     active_text,   black},
    .stepPopupBgLabelColor =             { active_text,   black,        two_true_red,    black},
    .stepPopupDescriptionTextBoxColors = { active_text,   black,        two_true_red},

    .notePopupColors =                   { black,         active_text, two_true_red},
	                                     // text          bg            text 2
    .noteKeyboardColors =                { active_text,   black,       active_text},

    .popupHorizontalBarColors =          { grey_22,       grey_22,      white,        grey_12},

    .popupTextColors =                   { active_text,   background,   two_true_red},

                                        // tlo           ramka         pasek/tekst
    .popupWithProgressColors =           { grey_22,      grey_22,      white},

	.noSdInfoTextColors =                { active_text, active_text},

	//------------------------------
                                        // 0            1           2            3             4             5              6               7               8
                                        // nazwy tr,    backgr,     nuta,        instr,        FX 1,         FX 2,          nuta nieakt,    instr nieakt,   FX 1 nieakt,
	.patternTrackerColors =              { patt_text,   grey_08,    patt_text,   patt_text,    patt_text,     patt_text,    patt_text2,     patt_text2,     patt_text2,
			                            // 9            10             11          12             13                14         15              16
			                            // FX 2 nieakt,   zaznaczenie,   podziałka,  track nieakt,  playhead,       txt zazn., podziałka pion  step num
			                               patt_text2,    two_true_red,  0x0e0e0e,   0x171717,      grey_22,        black,     black,          bright_text,
	                                    // 17               18         19                  20
								        // green mode       white mode     track aktywnny  trackend
								           one_true_green,  active_text,   dark_block,     instrYellow },

	.patternFillBgLabelColors =          { black,        black,        two_true_red,    black},

                                        // texty        activ bar/block  scroll     tlo        noact text    editmode        scroll bg   playhead/mute     emptytrack   playhead/activ bar
    .songPlayerColors =                  { white,       white,           grey_57,   black,     grey_33,      one_true_red,   black,      grey_44,          grey_12,     one_true_green},


	.configItemEditorTextLabelColors =   { active_text,  grey_X0A,      two_true_red,      active_text},
	.gridEditorColors =                  { grey_X0A,     black,         two_true_red},
    .configConfirmPopUpLabelColors =     { active_text,  grey_X22,      two_true_red,      black},

                                        // kolor główny,    kolor dod. 1,    kolor dod. 2,    kontener,    tło
    .sampleLoadMemBarNormalColors =      { active_object,   two_true_red,    one_true_green,  black,       black},
    .sampleLoadMemBarFullColors =        { two_true_red,    two_true_red,    two_true_red,    black,       black},


    .perfFxNameTextLabelColors =         { bright_text,      grey_X12,      two_true_red,        black},
    .perfValueNormalMLabelColors =       { bright_text,      black,         one_true_green       },
    .perfValueEditMLabelColors =         { bright_text,      black,         two_true_red         },

	.startScreenColors =                 { active_text,      active_object},

	//------------------------------

    .waveformColors =                    { active_object,    grey_77},
    .waveformPointsColors =              { white,            grey_0A,       two_true_red,        one_true_green},
    .waveformProgressColors =            { black,            two_true_red},
    .granularProgressColors =            { cyan,             grey_X22},
    .waveTablePositionColors =           { black,            black},
    .sliceColors =                       { instrYellow,         two_true_red,  one_true_green  },

	//------------------------------

	.batteryIconColors =                 { grey_90, white, red},

	.defaultTextColors =                 { active_text,        black,         two_true_red,         active_text},


    // bars colors
    .white = active_object,
    .red = two_true_red,
	.orange = orange,
	.yellow = instrYellow,
	.green = one_true_green,
    .cyan = dark_cyan,
    .grey_22 = inactive_object,

    .bottomScreenShadow = 0,
    .barShadow = 0,
    .patternEditorPlayheadShadow = 0,
    .patternEditorBottomButtonsLines = 0,
    .moreButtonColors =                  { active_text,   grey_X32,      active_text,      grey_X32},
};


template <size_t N>
static void setThemeColors(uint32_t (&colors)[N], uint32_t text, uint32_t background,
                           uint32_t frame, uint32_t secondary)
{
    colors[0] = text;
    if (N > 1) colors[1] = background;
    if (N > 2) colors[2] = frame;
    if (N > 3) colors[3] = secondary;
    for (size_t i = 4; i < N; ++i) colors[i] = background;
}

static Theme makePaletteTheme(uint32_t background, uint32_t surface, uint32_t text,
                              uint32_t muted, uint32_t accent, uint32_t accent2)
{
    Theme t = themeDefault;
    t.backgroundColor = background;
    setThemeColors(t.topBarLabelColors, text, surface, accent, text);
    setThemeColors(t.songTimerLabelColors, text, surface, accent2, text);
    setThemeColors(t.activeLabelsColors, text, surface, accent, text);
    setThemeColors(t.inactiveLabelsColors, muted, surface, accent, muted);
    setThemeColors(t.activeButtonLabelsColors, text, surface, accent, text);
    setThemeColors(t.inactiveButtonLabelsColors, muted, surface, accent, muted);
    setThemeColors(t.highlightedButtonLabelsColors, text, accent, accent, text);
    setThemeColors(t.redButtonLabelsColors, text, accent, accent, text);
    setThemeColors(t.masterEditTrackNameLabelsColors, text, surface, accent2, accent);
    setThemeColors(t.redBlinkLabelColors, accent, accent, accent, accent);
    setThemeColors(t.performActiveTrackLabelsColors, text, accent, accent, text);
    setThemeColors(t.performMutedTrackLabelsColors, muted, surface, accent, muted);
    setThemeColors(t.blankSpaceCoverLabelColors, text, background, accent, text);
    setThemeColors(t.patchEditSynthModelIconColors, text, surface, accent, text);
    t.bottomButtonsBgColors[0] = accent;
    t.bottomButtonsBgColors[1] = surface;
    t.bottomButtonsBgColors[2] = accent;
    t.miniBoxMenuColors[0] = text; t.miniBoxMenuColors[1] = surface;
    t.miniBoxMenuColors[2] = accent; t.miniBoxMenuColors[3] = background;
    t.miniBoxMenuColors[4] = accent; t.miniBoxMenuColors[5] = muted;
    t.miniBoxMenuColors[6] = text;
    t.imgGridColors[0] = text; t.imgGridColors[1] = surface;
    t.imgGridColors[2] = muted; t.imgGridColors[3] = accent;
    t.imgGridColors[4] = surface; t.imgGridColors[5] = accent;
    t.activeBarColors[0] = accent; t.activeBarColors[1] = accent2;
    t.activeBarColors[2] = accent; t.activeBarColors[3] = surface; t.activeBarColors[4] = background;
    t.inactiveBarColors[0] = muted; t.inactiveBarColors[1] = accent2;
    t.inactiveBarColors[2] = accent; t.inactiveBarColors[3] = surface; t.inactiveBarColors[4] = background;
    setThemeColors(t.activeListColors, text, surface, accent, text);
    t.activeListColors[3] = surface; t.activeListColors[4] = text; t.activeListColors[5] = text;
    setThemeColors(t.inactiveListColors, muted, surface, accent, muted);
    t.inactiveListColors[3] = surface;
    setThemeColors(t.projectActiveListColors, text, surface, accent, text);
    t.projectActiveListColors[3] = surface; t.projectActiveListColors[5] = accent2;
    setThemeColors(t.projectInactiveListColors, muted, surface, accent, muted);
    t.projectInactiveListColors[3] = surface; t.projectInactiveListColors[5] = accent2;
    setThemeColors(t.sampleLoaderActiveListColors, text, surface, accent, text);
    t.sampleLoaderActiveListColors[3] = surface;
    setThemeColors(t.sampleLoaderInactiveListColors, muted, surface, accent, muted);
    t.sampleLoaderInactiveListColors[3] = surface;
    t.editBoxColors[0] = text; t.editBoxColors[1] = surface;
    t.editBoxColors[2] = accent; t.editBoxColors[3] = accent2; t.editBoxColors[4] = background;
    t.frameColors[0] = accent; t.frameColors[1] = muted;
    t.greenFrameColors[0] = accent2; t.greenFrameColors[1] = muted;
    t.keyboardColors[0] = surface; t.keyboardColors[1] = text; t.keyboardColors[2] = accent;
    setThemeColors(t.stepPopupTitleLabelColor, text, surface, accent, text);
    setThemeColors(t.stepPopupListColor, text, surface, accent, text);
    t.stepPopupListColor[3] = surface;
    setThemeColors(t.stepPopupBgLabelColor, text, surface, accent, text);
    setThemeColors(t.stepPopupDescriptionTextBoxColors, text, surface, accent, text);
    t.notePopupColors[0] = surface; t.notePopupColors[1] = text; t.notePopupColors[2] = accent;
    t.noteKeyboardColors[0] = muted; t.noteKeyboardColors[1] = surface; t.noteKeyboardColors[2] = text;
    t.popupHorizontalBarColors[0] = surface; t.popupHorizontalBarColors[1] = surface;
    t.popupHorizontalBarColors[2] = text; t.popupHorizontalBarColors[3] = background;
    t.popupTextColors[0] = text; t.popupTextColors[1] = surface; t.popupTextColors[2] = background;
    t.popupWithProgressColors[0] = surface; t.popupWithProgressColors[1] = accent;
    t.popupWithProgressColors[2] = text;
    t.noSdInfoTextColors[0] = text; t.noSdInfoTextColors[1] = text;
    uint32_t pattern[21] = { text, background, accent2, accent, accent, accent2,
                             muted, muted, muted, muted, accent, surface, surface,
                             background, text, surface, muted, accent2, muted, text, accent };
    for (size_t i = 0; i < 21; ++i) t.patternTrackerColors[i] = pattern[i];
    setThemeColors(t.patternFillBgLabelColors, text, background, accent, text);
    t.songPlayerColors[0] = text; t.songPlayerColors[1] = accent;
    t.songPlayerColors[2] = muted; t.songPlayerColors[3] = background;
    t.songPlayerColors[4] = muted; t.songPlayerColors[5] = accent;
    t.songPlayerColors[6] = surface; t.songPlayerColors[7] = accent2;
    t.songPlayerColors[8] = surface; t.songPlayerColors[9] = accent;
    setThemeColors(t.configItemEditorTextLabelColors, text, background, accent, text);
    t.gridEditorColors[0] = surface; t.gridEditorColors[1] = text; t.gridEditorColors[2] = accent;
    setThemeColors(t.configConfirmPopUpLabelColors, text, surface, accent, text);
    t.sampleLoadMemBarNormalColors[0] = accent; t.sampleLoadMemBarNormalColors[1] = accent2;
    t.sampleLoadMemBarNormalColors[2] = accent; t.sampleLoadMemBarNormalColors[3] = surface;
    t.sampleLoadMemBarNormalColors[4] = background;
    t.sampleLoadMemBarFullColors[0] = accent; t.sampleLoadMemBarFullColors[1] = accent;
    t.sampleLoadMemBarFullColors[2] = accent; t.sampleLoadMemBarFullColors[3] = surface;
    t.sampleLoadMemBarFullColors[4] = background;
    setThemeColors(t.perfFxNameTextLabelColors, text, surface, accent, text);
    setThemeColors(t.perfValueNormalMLabelColors, text, background, accent2, text);
    setThemeColors(t.perfValueEditMLabelColors, text, background, accent, text);
    t.startScreenColors[0] = text; t.startScreenColors[1] = accent;
    t.waveformColors[0] = accent; t.waveformColors[1] = background;
    t.waveformPointsColors[0] = text; t.waveformPointsColors[1] = background;
    t.waveformPointsColors[2] = accent; t.waveformPointsColors[3] = accent2;
    t.waveformProgressColors[0] = text; t.waveformProgressColors[1] = accent;
    t.granularProgressColors[0] = accent2; t.granularProgressColors[1] = surface;
    t.waveTablePositionColors[0] = text; t.waveTablePositionColors[1] = surface;
    t.sliceColors[0] = accent2; t.sliceColors[1] = accent; t.sliceColors[2] = text;
    t.batteryIconColors[0] = muted; t.batteryIconColors[1] = accent2; t.batteryIconColors[2] = accent;
    t.defaultTextColors[0] = text; t.defaultTextColors[1] = surface;
    t.defaultTextColors[2] = accent; t.defaultTextColors[3] = text;
    t.white = text; t.red = accent; t.orange = accent2; t.yellow = accent2;
    t.green = accent2; t.cyan = accent; t.grey_22 = muted;
    t.moreButtonColors[0] = text; t.moreButtonColors[1] = surface;
    t.moreButtonColors[2] = text; t.moreButtonColors[3] = muted;
    t.bottomScreenShadow = 0; t.barShadow = 0;
    return t;
}

const Theme& getBuiltInTheme(uint8_t index)
{
    static const Theme themes[] = {
        themeDefault, themeWhiteout, themeAnniversary,
        makePaletteTheme(0x80808, 0x202020, 0xF2F2F2, 0xB0B0B0, 0x35D07F, 0xD7D7D7),
        makePaletteTheme(0x20B05, 0x92014, 0xD7FFE3, 0x75B88A, 0xFF66, 0xB5FF00),
        makePaletteTheme(0x50B20, 0x101F3D, 0xE4F2FF, 0x829DBB, 0x22A7FF, 0xE5FF),
        makePaletteTheme(0x160D05, 0x2B1B0B, 0xFFE4B5, 0xB89A72, 0xFFB347, 0xE87932),
        makePaletteTheme(0x10051A, 0x281039, 0xF8E8FF, 0xB89AC8, 0xD94BFF, 0x7B5CFF),
        makePaletteTheme(0x160606, 0x301010, 0xFFE7E7, 0xB88A8A, 0xFF3030, 0xFF8A40),
        makePaletteTheme(0x7121C, 0x122A3A, 0xE8FFFF, 0x82B8C2, 0xFF4FA3, 0x20E0D0),
        makePaletteTheme(0x10051F, 0x26113D, 0xF7E9FF, 0xB69ACB, 0xFF3FA4, 0x35D8FF),
        makePaletteTheme(0x27245D, 0x383477, 0xA8A8FF, 0x7777C9, 0x8A8AFF, 0xD6D6FF),
        makePaletteTheme(0x101010, 0x252525, 0xF5F5F5, 0xA0A0A0, 0xE6E6E6, 0x70C0A0),
        makePaletteTheme(0x80812, 0x1A1A2C, 0xFFFFFF, 0x9999B8, 0xFF3030, 0xE6FF),
        makePaletteTheme(0xDCE7EF, 0xC3D2DF, 0x152536, 0x526A7E, 0x87FC1, 0xA6A6),
        makePaletteTheme(0x31521, 0x82B3B, 0xD8F5FF, 0x78A9B8, 0xB9D6, 0x187BFF),
        makePaletteTheme(0x7150D, 0x14291B, 0xE1F5E5, 0x91B09A, 0x52C878, 0xC0A85A),
        makePaletteTheme(0x1B0B14, 0x351727, 0xFFE9D8, 0xC29AA4, 0xFF765E, 0xFFC15A),
        makePaletteTheme(0x1A100B, 0x302018, 0xF7E3D3, 0xB89B88, 0xD98955, 0xF0B878),
        makePaletteTheme(0xB1003, 0x202B08, 0xF2FFD6, 0xA4B77C, 0xB8FF00, 0x48D66A),
        makePaletteTheme(0x180713, 0x321027, 0xFFEAF7, 0xC49AB8, 0xFF35B8, 0xFF8CCF),
        makePaletteTheme(0x90B0D, 0x191D21, 0xE3E7EA, 0x89939B, 0x8796A5, 0xC0CBD4),
        makePaletteTheme(0x171206, 0x30250B, 0xFFF2C7, 0xB9A36D, 0xE8B923, 0xFFF0A0),
    };
    return themes[index < (sizeof(themes) / sizeof(themes[0])) ? index : 0];
}

Theme themeCustom = themeDefault;

// clang-format on
