<map version="freeplane 1.9.13">
<!--To view this file, download free mind mapping software Freeplane from https://www.freeplane.org -->
<node TEXT="nc-cli" LOCALIZED_STYLE_REF="AutomaticLayout.level.root" FOLDED="false" ID="ID_1090958577" CREATED="1409300609620" MODIFIED="1670057248242"><hook NAME="MapStyle" background="#2e3440" zoom="2.197">
    <properties show_icon_for_attributes="true" edgeColorConfiguration="#808080ff,#ff0000ff,#0000ffff,#00ff00ff,#ff00ffff,#00ffffff,#7c0000ff,#00007cff,#007c00ff,#7c007cff,#007c7cff,#7c7c00ff" show_note_icons="true" fit_to_viewport="false" associatedTemplateLocation="template:/dark_nord_template.mm"/>

<map_styles>
<stylenode LOCALIZED_TEXT="styles.root_node" STYLE="oval" UNIFORM_SHAPE="true" VGAP_QUANTITY="24 pt">
<font SIZE="24"/>
<stylenode LOCALIZED_TEXT="styles.predefined" POSITION="right" STYLE="bubble">
<stylenode LOCALIZED_TEXT="default" ID="ID_671184412" ICON_SIZE="12 pt" FORMAT_AS_HYPERLINK="false" COLOR="#484747" BACKGROUND_COLOR="#eceff4" STYLE="bubble" SHAPE_HORIZONTAL_MARGIN="8 pt" SHAPE_VERTICAL_MARGIN="5 pt" BORDER_WIDTH_LIKE_EDGE="false" BORDER_WIDTH="1.9 px" BORDER_COLOR_LIKE_EDGE="true" BORDER_COLOR="#f0f0f0" BORDER_DASH_LIKE_EDGE="true" BORDER_DASH="SOLID">
<arrowlink SHAPE="CUBIC_CURVE" COLOR="#88c0d0" WIDTH="2" TRANSPARENCY="255" DASH="" FONT_SIZE="9" FONT_FAMILY="SansSerif" DESTINATION="ID_671184412" STARTARROW="NONE" ENDARROW="DEFAULT"/>
<font NAME="SansSerif" SIZE="11" BOLD="false" STRIKETHROUGH="false" ITALIC="false"/>
<edge STYLE="bezier" COLOR="#81a1c1" WIDTH="3" DASH="SOLID"/>
<richcontent CONTENT-TYPE="plain/auto" TYPE="DETAILS"/>
<richcontent TYPE="NOTE" CONTENT-TYPE="plain/auto"/>
</stylenode>
<stylenode LOCALIZED_TEXT="defaultstyle.details" BORDER_WIDTH="1.9 px">
<edge STYLE="bezier" COLOR="#81a1c1" WIDTH="3"/>
</stylenode>
<stylenode LOCALIZED_TEXT="defaultstyle.attributes">
<font SIZE="10"/>
</stylenode>
<stylenode LOCALIZED_TEXT="defaultstyle.note" COLOR="#000000" BACKGROUND_COLOR="#ebcb8b">
<icon BUILTIN="clock2"/>
<font SIZE="10"/>
</stylenode>
<stylenode LOCALIZED_TEXT="defaultstyle.floating" COLOR="#484747">
<edge STYLE="hide_edge"/>
<cloud COLOR="#f0f0f0" SHAPE="ROUND_RECT"/>
</stylenode>
<stylenode LOCALIZED_TEXT="defaultstyle.selection" COLOR="#e5e9f0" BACKGROUND_COLOR="#5e81ac" BORDER_COLOR_LIKE_EDGE="false" BORDER_COLOR="#5e81ac"/>
</stylenode>
<stylenode LOCALIZED_TEXT="styles.user-defined" POSITION="right" STYLE="bubble">
<stylenode LOCALIZED_TEXT="styles.important" ID="ID_779275544" BORDER_COLOR_LIKE_EDGE="false" BORDER_COLOR="#bf616a">
<icon BUILTIN="yes"/>
<arrowlink COLOR="#bf616a" TRANSPARENCY="255" DESTINATION="ID_779275544"/>
<font SIZE="14"/>
</stylenode>
</stylenode>
<stylenode LOCALIZED_TEXT="styles.AutomaticLayout" POSITION="right" STYLE="bubble">
<stylenode LOCALIZED_TEXT="AutomaticLayout.level.root" COLOR="#ffffff" BACKGROUND_COLOR="#484747" STYLE="bubble" SHAPE_HORIZONTAL_MARGIN="10 pt" SHAPE_VERTICAL_MARGIN="10 pt">
<font NAME="Ubuntu" SIZE="18"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,1" COLOR="#eceff4" BACKGROUND_COLOR="#d08770" STYLE="bubble" SHAPE_HORIZONTAL_MARGIN="8 pt" SHAPE_VERTICAL_MARGIN="5 pt">
<font NAME="Ubuntu" SIZE="16"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,2" COLOR="#3b4252" BACKGROUND_COLOR="#ebcb8b">
<font SIZE="14"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,3" COLOR="#2e3440" BACKGROUND_COLOR="#a3be8c">
<font SIZE="12"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,4" COLOR="#2e3440" BACKGROUND_COLOR="#b48ead">
<font SIZE="11"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,5" BACKGROUND_COLOR="#81a1c1">
<font SIZE="10"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,6" BACKGROUND_COLOR="#88c0d0">
<font SIZE="10"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,7" BACKGROUND_COLOR="#8fbcbb">
<font SIZE="10"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,8" BACKGROUND_COLOR="#d8dee9">
<font SIZE="10"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,9" BACKGROUND_COLOR="#e5e9f0">
<font SIZE="9"/>
</stylenode>
<stylenode LOCALIZED_TEXT="AutomaticLayout.level,10" BACKGROUND_COLOR="#eceff4">
<font SIZE="9"/>
</stylenode>
</stylenode>
</stylenode>
</map_styles>
</hook>
<hook NAME="accessories/plugins/AutomaticLayout.properties" VALUE="ALL"/>
<font BOLD="true"/>
<node TEXT="nc_cli_config config&#xa;全局结构体" POSITION="right" ID="ID_1788265467" CREATED="1670057254453" MODIFIED="1670057495808">
<node TEXT="name" ID="ID_1626433850" CREATED="1670057506559" MODIFIED="1670057510318">
<node TEXT="数据库名称" ID="ID_202744467" CREATED="1670060163313" MODIFIED="1670060168914"/>
</node>
<node TEXT="type" ID="ID_767138705" CREATED="1670057510532" MODIFIED="1670057513574"/>
<node TEXT="module" ID="ID_1734841521" CREATED="1670057513780" MODIFIED="1670057516422"/>
<node TEXT="origin" ID="ID_872279942" CREATED="1670057516605" MODIFIED="1670057519518"/>
<node TEXT="dnode" ID="ID_1817310896" CREATED="1670057519996" MODIFIED="1670057522022">
<node TEXT="存储配置树" ID="ID_620380385" CREATED="1670057571807" MODIFIED="1670057588345"/>
</node>
<node TEXT="path&#xa;存储当前路径" ID="ID_1811621650" CREATED="1670057522236" MODIFIED="1670057787304">
<node TEXT="len" ID="ID_1724735159" CREATED="1670057606695" MODIFIED="1670057707096"/>
<node TEXT="[char str]" ID="ID_163486994" CREATED="1670057708797" MODIFIED="1670057740600"/>
</node>
</node>
<node TEXT="命令类型" POSITION="left" ID="ID_659742639" CREATED="1670058820096" MODIFIED="1670058831817">
<node TEXT="standard 标准命令" ID="ID_381334511" CREATED="1670058837224" MODIFIED="1670058912985"/>
<node TEXT="hidden 隐藏命令" ID="ID_539518953" CREATED="1670058852694" MODIFIED="1670058905308"/>
<node TEXT="contextual 上下文命令" ID="ID_1681217632" CREATED="1670058872597" MODIFIED="1670058898265"/>
</node>
<node TEXT="?&#xa;help&#xa;help xxx&#xa;都是跳转到cmd_help" POSITION="left" ID="ID_559916452" CREATED="1670058624637" MODIFIED="1670059034977"/>
<node TEXT="edit running 编辑模式" POSITION="right" ID="ID_517379954" CREATED="1670061664711" MODIFIED="1670061677322">
<node TEXT="cmd_edit" ID="ID_1899544483" CREATED="1670063728593" MODIFIED="1670063734483">
<node TEXT="加载配置树" ID="ID_626631762" CREATED="1670063740051" MODIFIED="1670063750476"/>
</node>
<node TEXT="nc_cli_feature_refresh" ID="ID_1768254908" CREATED="1670063754977" MODIFIED="1670063800356">
<node TEXT=" 更新rpc命令" ID="ID_890670079" CREATED="1670062915146" MODIFIED="1670063811180"/>
</node>
<node TEXT="nc_cli_context_set" ID="ID_1335907107" CREATED="1670063872720" MODIFIED="1670063880204">
<node TEXT="nc_cli_config_post_parse()&#xa;后退时调用切换路径&#xa;nc-cli_config_ctx_get_prompt()&#xa;打印路径" ID="ID_247946222" CREATED="1670060643727" MODIFIED="1670061986075"/>
</node>
<node TEXT="cmd_del" ID="ID_1824249306" CREATED="1670061909799" MODIFIED="1670061914434">
<node TEXT="cmd_del_path 删除某个路径" ID="ID_1333775599" CREATED="1670061727671" MODIFIED="1670061758915"/>
<node TEXT="cmd_path 进入某个路径" ID="ID_888972652" CREATED="1670061379761" MODIFIED="1670061720242"/>
</node>
<node TEXT="cmd_set" ID="ID_1027510139" CREATED="1670061872663" MODIFIED="1670061875729"/>
<node TEXT="cmd_set_oneliner" ID="ID_1566844628" CREATED="1670061857759" MODIFIED="1670061867027"/>
<node TEXT="cmd_commit" ID="ID_609755122" CREATED="1670064005024" MODIFIED="1670064169028">
<node TEXT="nc_cli_netconf_apply" ID="ID_1150637497" CREATED="1670064171106" MODIFIED="1670064201923"/>
</node>
</node>
<node TEXT="netconf connect" POSITION="right" ID="ID_1219271608" CREATED="1670062774442" MODIFIED="1670062785372">
<node TEXT="nc_cli_feature_refresh 获取rpc命令" ID="ID_1378382021" CREATED="1670062915146" MODIFIED="1670062933739"/>
</node>
<node TEXT="cmd 模式" POSITION="right" ID="ID_1161144008" CREATED="1670062316512" MODIFIED="1670062321851">
<node TEXT="nc_cli_cmp_rpc" ID="ID_1396573837" CREATED="1670062324809" MODIFIED="1670062335387"/>
</node>
</node>
</map>
