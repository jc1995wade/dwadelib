<map version="freeplane 1.9.13">
<!--To view this file, download free mind mapping software Freeplane from https://www.freeplane.org -->
<node TEXT="Redis" LOCALIZED_STYLE_REF="AutomaticLayout.level.root" FOLDED="false" ID="ID_1090958577" CREATED="1409300609620" MODIFIED="1675500169511"><hook NAME="MapStyle" background="#2e3440" zoom="1.69">
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
<node TEXT="特点" POSITION="right" ID="ID_722878081" CREATED="1675500192862" MODIFIED="1675500197087">
<node TEXT="非结构化" ID="ID_1250035034" CREATED="1675500258988" MODIFIED="1675500271253"/>
<node TEXT="数据无关联" ID="ID_1277348490" CREATED="1675500271501" MODIFIED="1675500292302"/>
<node TEXT="非SQL语句" ID="ID_831876439" CREATED="1675500292572" MODIFIED="1675500307022"/>
<node TEXT="事务特性" ID="ID_241564635" CREATED="1675500299566" MODIFIED="1675500319080">
<node TEXT="不符合ACID" ID="ID_1824302394" CREATED="1675500330736" MODIFIED="1675500338552"/>
</node>
</node>
<node TEXT="高可用" POSITION="right" ID="ID_1417148905" CREATED="1675500200649" MODIFIED="1675500246291">
<node TEXT="主从复制" ID="ID_1137322487" CREATED="1675500365741" MODIFIED="1675500382458">
<node TEXT="1 salve 向master发送sync命令&#xa;2 master启动后台存盘进程，并手动所有修改数据命令&#xa;3 master完成存盘后，传送整个数据文件到slave&#xa;4 slave接受数据文件，加载到内存中完成首次完全同步&#xa;5 后续有新数据产生时，master继续将新的数据命令传递给slave完成同步" ID="ID_1361487933" CREATED="1675500864424" MODIFIED="1675500876551"/>
<node TEXT="同步模式:slave发送PSYNC" ID="ID_1100651451" CREATED="1675501197324" MODIFIED="1675501342718">
<node TEXT="完整重同步：处理初次复制情况，从服务器（Slave）先让主服务器（Master）创建并发送RDB文件，然后再将主服务器的缓冲区里的命令发送给从服务器，以此完成同步；" ID="ID_1885516091" CREATED="1675501212676" MODIFIED="1675501421609"/>
<node TEXT="部分重同步：用于处理断线后复制的情况，在断线重连后，只要将断线期间主服务器产生的命令发送给从服务器，就可以完成数据同步了。" ID="ID_1742601218" CREATED="1675501238195" MODIFIED="1675501413624"/>
</node>
</node>
<node TEXT="哨兵" ID="ID_1403856338" CREATED="1675500382695" MODIFIED="1675500390411">
<node TEXT="集群数量不得少于3个节点" ID="ID_1429055277" CREATED="1675500558043" MODIFIED="1675500572954"/>
<node TEXT="作用" ID="ID_1406996802" CREATED="1675500582920" MODIFIED="1675500586760">
<node TEXT="监控" ID="ID_86456910" CREATED="1675500588433" MODIFIED="1675500595221"/>
<node TEXT="自动故障转移" ID="ID_529743818" CREATED="1675500595426" MODIFIED="1675500602378"/>
<node TEXT="通知提醒" ID="ID_1722370111" CREATED="1675500602562" MODIFIED="1675500614308"/>
</node>
<node TEXT="哨兵的启动依赖于主从模式，所以须把主从模式安装好的情况下再去做哨兵模式，所有节点上都需要部署哨兵模式，哨兵模式会监控所有的 Redis 工作节点是否正常，当 Master 出现问题的时候，因为其他节点与主节点失去联系，因此会投票，投票过半就认为这个 Master 的确出现问题，然后会通知哨兵间，然后从 Slaves 中选取一个作为新的 Master" ID="ID_829163459" CREATED="1675500680732" MODIFIED="1675500690761"/>
</node>
<node TEXT="集群" ID="ID_516592789" CREATED="1675500390674" MODIFIED="1675500395538">
<node TEXT="所有的redis节点彼此互联(PING-PONG机制),内部使用二进制协议优化传输速度和带宽" ID="ID_1120760653" CREATED="1675587482522" MODIFIED="1675587521595"/>
<node TEXT="节点的fail是通过集群中超过半数的节点检测失效时才生效。" ID="ID_91037951" CREATED="1675587485080" MODIFIED="1675587518588"/>
<node TEXT="客户端与redis节点直连,不需要中间proxy层.客户端不需要连接集群所有节点,连接集群中任何一个可用节点即可。" ID="ID_1408991637" CREATED="1675587498481" MODIFIED="1675587514617"/>
<node TEXT="redis-cluster把所有的物理节点映射到[0-16383]slot上" ID="ID_1131592182" CREATED="1675587506614" MODIFIED="1675587511765"/>
</node>
</node>
<node TEXT="发布/订阅" POSITION="left" ID="ID_927765396" CREATED="1675502267751" MODIFIED="1675502275854">
<node TEXT="客户端执行订阅以后,除了可以继续订阅(SUBSCRIBE或者PSUBSCRIBE),取消订阅(UNSUBSCRIBE或者PUNSUBSCRIBE), PING命令和结束连接(QUIT)外, 不能执行其他操作,客户端将阻塞直到订阅通道上发布消息的到来." ID="ID_707068085" CREATED="1675502288371" MODIFIED="1675587416130"/>
<node TEXT="发布的消息在Redis系统中不存储.因此,必须先执行订阅,再等待消息发布. 但是,相反的顺序则不支持." ID="ID_389681921" CREATED="1675587377280" MODIFIED="1675587412753"/>
<node TEXT="订阅的通道名称支持glob模式匹配.如果客户端同时订阅了glob模式的通道和非glob模式的通道,并且名称存在交集,则对于一个发布的消息,该执行订阅的客户端接收到两个消息." ID="ID_1866970145" CREATED="1675587390039" MODIFIED="1675587410586"/>
<node TEXT="在发布/订阅模式下，通道名称是全局的和客户端连接的Redis数据库没有关系。" ID="ID_199437400" CREATED="1675587398140" MODIFIED="1675587408612"/>
</node>
</node>
</map>
