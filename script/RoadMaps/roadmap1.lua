local nodes = Collection:new()
  
local node_data = {{vec(66.990740264417, 49.24065023921), 2.187500},
{vec(64.977708373729, 46.764591008559), 4.687500},
{vec(68.071428571429, 42.892857142857), 4.687500},
{vec(59.977753386333, 46.77314340323), 4.687500},
{vec(66.428571428571, 38.178571428571), 2.187500},
{vec(54.977756848841, 46.773801279743), 4.687500},
{vec(55.75, 41.875), 2.187500},
{vec(66.654230049427, 35.994637458611), 2.187500},
{vec(49.990295084822, 49.156066116095), 2.187500},
{vec(54, 40), 3.437500},
{vec(48, 35.142857142857), 8.437500},
{vec(66.431511116449, 33.001557503791), 2.187500},
{vec(46.990704650049, 49.233883509361), 2.187500},
{vec(48, 40), 4.062500},
{vec(42.765481651376, 28.435534076016), 4.687500},
{vec(39.5, 38.25), 1.562500},
{vec(55.390999969429, 30.834367643912), 2.187500},
{vec(43.5, 42), 4.687500},
{vec(65.378905716497, 31.008555017679), 2.187500},
{vec(66.434250821912, 29.001549997748), 2.187500},
{vec(42.357142857143, 47.375), 3.437500},
{vec(43.989048581955, 48.919230571373), 2.187500},
{vec(38.5, 24), 2.812500},
{vec(38.593895619758, 42.143522833179), 2.187500},
{vec(65.392677208371, 27.008365327708), 2.187500},
{vec(66.436990527375, 25.001542491706), 2.187500},
{vec(37.992681247453, 49.609437015991), 1.562500},
{vec(40.990740264417, 49.24065023921), 2.187500},
{vec(35.529351883271, 25.208347472005), 2.187500},
{vec(35.194239904988, 21.957957244656), 2.812500},
{vec(35.569431500466, 42.031686859273), 2.812500},
{vec(65.406448700244, 23.008175637738), 2.187500},
{vec(65.809004092769, 20.578444747613), 2.812500},
{vec(35.996527599156, 49.340243839704), 2.187500},
{vec(30.571428571429, 22.5), 2.187500},
{vec(33.589285714286, 16.571428571429), 2.812500},
{vec(35.154381443299, 18.178608247423), 2.187500},
{vec(32.764175257732, 20.313144329897), 2.187500},
{vec(33.608574091333, 43.210624417521), 2.187500},
{vec(64.857142857143, 17.75), 3.437500},
{vec(33.098353121409, 50.057296055151), 1.562500},
{vec(28.870109959822, 24.200095157539), 2.187500},
{vec(26.772044829774, 22.84119264115), 2.812500},
{vec(29.937611091362, 14.346693921081), 2.187500},
{vec(37.178571428571, 11.5), 3.437500},
{vec(33.75, 12.375), 2.812500},
{vec(30, 43.125), 2.812500},
{vec(62.285714285714, 11), 5.937500},
{vec(61.36878776067, 19.682079516663), 1.562500},
{vec(31.056989659134, 49.612638835695), 1.562500},
{vec(23.794063226898, 23.146304715585), 2.812500},
{vec(26.952008531817, 14.728226093139), 2.187500},
{vec(39.59005451107, 8.8836911781065), 2.812500},
{vec(37.214285714286, 13.5), 1.562500},
{vec(24.977403673028, 46.706697875405), 4.687500},
{vec(56.299433962264, 11.496981132075), 4.062500},
{vec(56.5, 15.5), 2.812500},
{vec(54.492843665768, 7.6261185983827), 2.812500},
{vec(65.310237947099, 5.0232749594064), 2.812500},
{vec(27.992862287155, 49.643834559391), 1.562500},
{vec(18.214285714286, 23.5), 2.812500},
{vec(20.876770987524, 24.292397969973), 2.187500},
{vec(24.023841739027, 14.168245212323), 2.812500},
{vec(42.916703749027, 6.3060824340861), 2.812500},
{vec(25.991185444012, 49.325234362325), 2.187500},
{vec(19.977687598681, 46.760643749481), 2.812500},
{vec(22.557692307692, 41.961538461538), 2.187500},
{vec(51.537789757412, 9.5920215633423), 2.187500},
{vec(51.678571428571, 5.6607142857143), 2.812500},
{vec(48.75, 3), 4.687500},
{vec(65.372209660531, 2.0224213545381), 2.812500},
{vec(15.125, 23.375), 2.812500},
{vec(18.967434975682, 25.548741805879), 1.562500},
{vec(21.929612513331, 15.134731603271), 2.187500},
{vec(16.980679205562, 47.329049056813), 2.812500},
{vec(21.384615384615, 40.076923076923), 2.187500},
{vec(20.557692307692, 37.961538461538), 2.812500},
{vec(49.384582210243, 7.7082479784367), 1.562500},
{vec(51.285714285714, -1), 2.187500},
{vec(48.75, -2), 3.437500},
{vec(66.478060457987, -0.99281075011002), 1.562500},
{vec(12.786813186813, 19.151648351648), 2.812500},
{vec(10.121434078643, 21.628373168851), 3.437500},
{vec(12.607142857143, 25.625), 2.812500},
{vec(18.931745467472, 15.19125488802), 2.187500},
{vec(13.985966455223, 48.333626492341), 2.812500},
{vec(18.557692307692, 35.961538461538), 2.187500},
{vec(51.5, -7.75), 4.062500},
{vec(66.491831949861, -2.9930004400807), 1.562500},
{vec(13.392857142857, 15.928571428571), 2.187500},
{vec(8.0393215111796, 24.584425597533), 2.187500},
{vec(5.5, 23), 2.812500},
{vec(14.557692307692, 29.961538461538), 2.187500},
{vec(13.384615384615, 28.076923076923), 2.187500},
{vec(15.925346605048, 15.021685033772), 2.187500},
{vec(10.989315689712, 48.969981045242), 2.187500},
{vec(17.384615384615, 34.076923076923), 2.187500},
{vec(16.557692307692, 31.961538461538), 2.812500},
{vec(56, -10), 1.562500},
{vec(49.428571428571, -11.428571428571), 5.937500},
{vec(66.505603441735, -4.9931901300514), 1.562500},
{vec(13.875, 12.625), 3.437500},
{vec(6.0123361603701, 25.555898226677), 1.562500},
{vec(0.21396692045156, 28.027435022316), 4.687500},
{vec(7.9907847823765, 49.249108651521), 2.187500},
{vec(57, -12), 2.812500},
{vec(55, -14), 2.812500},
{vec(42.214285714286, -11.875), 5.312500},
{vec(66.519374933609, -6.9933798200222), 1.562500},
{vec(14.589285714286, 7.125), 4.062500},
{vec(-1.8419445361505, 33.65347474414), 5.312500},
{vec(-4.6249835912784, 29.004528866603), 4.687500},
{vec(4.9904731566597, 49.189899765341), 2.187500},
{vec(61, -11), 1.562500},
{vec(60, -13), 3.437500},
{vec(59, -11), 1.562500},
{vec(37, -10.5), 4.687500},
{vec(66.533146425482, -8.9935695099929), 1.562500},
{vec(10.214285714286, 7.3928571428571), 2.812500},
{vec(14.446428571429, 3.0357142857143), 3.437500},
{vec(11.276144733716, 4.351693764638), 2.187500},
{vec(0.69220267786821, 39.023969545813), 2.812500},
{vec(-7.1428571428571, 30.857142857143), 2.812500},
{vec(-11.5, 34.25), 4.687500},
{vec(-6.9335707211244, 36.17319483092), 2.812500},
{vec(-6.8497498661163, 24.878338928147), 2.812500},
{vec(-6.4624185075005, 21.964499335401), 3.437500},
{vec(1.9770435722002, 46.638278718041), 2.812500},
{vec(63, -11), 1.562500},
{vec(64, -13), 3.437500},
{vec(32, -10.5), 4.687500},
{vec(35, -15), 2.187500},
{vec(65, -10), 1.562500},
{vec(66, -12), 2.187500},
{vec(7.9637434149365, 8.2596839169507), 2.187500},
{vec(18.217409326425, -0.25015544041447), 2.812500},
{vec(1.2227533236247, 41.911072485555), 2.812500},
{vec(-13.407960336805, 27.993768003545), 2.187500},
{vec(-11.714285714286, 39), 2.187500},
{vec(-15.977469670711, 37.459272097054), 1.562500},
{vec(-14.326390427653, 29.992909372923), 2.187500},
{vec(-4.3571428571429, 22.75), 1.562500},
{vec(-8.2119672409514, 18.810881150485), 2.187500},
{vec(-7.8854086390888, 15.882110529134), 2.812500},
{vec(-5.2984234381327, 18.714256325187), 2.187500},
{vec(-0.0080369756279026, 49.472974630699), 2.187500},
{vec(0.56539774218955, 44.024888422158), 2.187500},
{vec(27.214285714286, -8.25), 2.187500},
{vec(27, -15), 2.187500},
{vec(30, -15), 2.187500},
{vec(28, -13), 1.562500},
{vec(28.724574389341, -6.3476683937824), 1.562500},
{vec(4.9640223117447, 8.2730709637434), 2.187500},
{vec(20.269559585492, -2.1843264248705), 2.812500},
{vec(-13.397435187237, 23.993657212497), 2.187500},
{vec(-14.305340128518, 25.992687790827), 2.187500},
{vec(-11.321428571429, 40.607142857143), 1.562500},
{vec(-11.821428571429, 43), 2.187500},
{vec(-18, 37.5), 1.562500},
{vec(-4.8670086819261, 16.098263614838), 2.187500},
{vec(-8, 10), 4.687500},
{vec(-8.5714285714286, 12.75), 2.187500},
{vec(-5.2709155485399, 13.066890292028), 2.187500},
{vec(-3.0082862762013, 49.425607521754), 2.187500},
{vec(25.954285714286, -5.32), 2.187500},
{vec(24, -15), 2.187500},
{vec(26, -13), 1.562500},
{vec(24, -13), 1.562500},
{vec(1.9651843817787, 8.3288503253796), 2.187500},
{vec(23.20792746114, -4.2621243523316), 2.812500},
{vec(-13.386910037669, 19.993546421449), 2.187500},
{vec(-14.381758253933, 21.982965876357), 2.187500},
{vec(-12.34549749861, 45.200611450806), 2.187500},
{vec(-10.020691947591, 47.068529957619), 2.812500},
{vec(-13.857142857143, 12.357142857143), 2.187500},
{vec(-3.25, 8.3214285714286), 2.187500},
{vec(-9.2857142857143, 5.2857142857143), 5.937500},
{vec(-14.267857142857, 9.5178571428571), 2.187500},
{vec(-7.0086186769658, 49.362451376495), 2.187500},
{vec(22, -13), 1.562500},
{vec(21, -15), 2.187500},
{vec(-1.0356987914472, 8.2864580105361), 2.187500},
{vec(-13.698791062725, 16.112184315463), 2.187500},
{vec(-14.601487435914, 18.039720868409), 1.562500},
{vec(-12.428571428571, 49.285714285714), 2.187500},
{vec(-14.720931704538, 14.113646433319), 1.562500},
{vec(-14.142857142857, -0.58928571428571), 2.812500},
{vec(-14.555423221804, 1.9847939286506), 2.187500},
{vec(-11.082628461221, -0.47698030031685), 2.187500},
{vec(20, -13), 1.562500},
{vec(18, -15), 2.187500},
{vec(18, -13), 1.562500},
{vec(-14.77731092437, -5.0378151260504), 2.187500},
{vec(16, -13), 1.562500},
{vec(15, -15), 2.187500},
{vec(-14.829718810601, -8.0389301874596), 2.187500},
{vec(14, -13), 1.562500},
{vec(12, -15), 2.187500},
{vec(12, -13), 1.562500},
{vec(-14, -11), 1.562500},
{vec(10, -13), 1.562500},
{vec(9, -15), 2.187500},
{vec(-11, -13), 2.187500},
{vec(-15, -13), 1.562500},
{vec(-13, -14), 2.812500},
{vec(8, -13), 1.562500},
{vec(6, -15), 2.187500},
{vec(6, -13), 1.562500},
{vec(-8, -13), 2.187500},
{vec(-9, -15), 2.187500},
{vec(3, -15), 2.187500},
{vec(4, -13), 2.187500},
{vec(-5, -13), 2.187500},
{vec(-6, -15), 2.187500},
{vec(1, -13), 2.187500},
{vec(0, -15), 2.187500},
{vec(-3, -15), 2.187500},
{vec(-2, -13), 2.187500}}

for _, n in pairs(node_data) do
  nodes:append(PrmNode:newNode(n[1], n[2]))
end

nodes[1]:insertNeighbors(nodes[2], nodes[3])
nodes[2]:insertNeighbors(nodes[4], nodes[3], nodes[1])
nodes[3]:insertNeighbors(nodes[4], nodes[5], nodes[2], nodes[1])
nodes[4]:insertNeighbors(nodes[6], nodes[3], nodes[2], nodes[7])
nodes[5]:insertNeighbors(nodes[3], nodes[8])
nodes[6]:insertNeighbors(nodes[4], nodes[9], nodes[7], nodes[10])
nodes[7]:insertNeighbors(nodes[6], nodes[11], nodes[10], nodes[4])
nodes[8]:insertNeighbors(nodes[5], nodes[12])
nodes[9]:insertNeighbors(nodes[6], nodes[13])
nodes[10]:insertNeighbors(nodes[14], nodes[11], nodes[7], nodes[6])
nodes[11]:insertNeighbors(nodes[15], nodes[14], nodes[16], nodes[10], nodes[17], nodes[7], nodes[18])
nodes[12]:insertNeighbors(nodes[19], nodes[8], nodes[20])
nodes[13]:insertNeighbors(nodes[21], nodes[9], nodes[22])
nodes[14]:insertNeighbors(nodes[10], nodes[11], nodes[18])
nodes[15]:insertNeighbors(nodes[11], nodes[23])
nodes[16]:insertNeighbors(nodes[18], nodes[11])
nodes[17]:insertNeighbors(nodes[11])
nodes[18]:insertNeighbors(nodes[24], nodes[11], nodes[14], nodes[16], nodes[21])
nodes[19]:insertNeighbors(nodes[25], nodes[20], nodes[12])
nodes[20]:insertNeighbors(nodes[19], nodes[26], nodes[12], nodes[25])
nodes[21]:insertNeighbors(nodes[27], nodes[22], nodes[28], nodes[18], nodes[13])
nodes[22]:insertNeighbors(nodes[21], nodes[13], nodes[28])
nodes[23]:insertNeighbors(nodes[29], nodes[15], nodes[30])
nodes[24]:insertNeighbors(nodes[18], nodes[31])
nodes[25]:insertNeighbors(nodes[19], nodes[32], nodes[20], nodes[26])
nodes[26]:insertNeighbors(nodes[25], nodes[32], nodes[20], nodes[33])
nodes[27]:insertNeighbors(nodes[21], nodes[34], nodes[28])
nodes[28]:insertNeighbors(nodes[21], nodes[27], nodes[22])
nodes[29]:insertNeighbors(nodes[23], nodes[30])
nodes[30]:insertNeighbors(nodes[35], nodes[36], nodes[29], nodes[23], nodes[37], nodes[38])
nodes[31]:insertNeighbors(nodes[39], nodes[24])
nodes[32]:insertNeighbors(nodes[26], nodes[33], nodes[40], nodes[25])
nodes[33]:insertNeighbors(nodes[26], nodes[32], nodes[40])
nodes[34]:insertNeighbors(nodes[27], nodes[41])
nodes[35]:insertNeighbors(nodes[42], nodes[30], nodes[43], nodes[38])
nodes[36]:insertNeighbors(nodes[38], nodes[30], nodes[44], nodes[37], nodes[45], nodes[46])
nodes[37]:insertNeighbors(nodes[38], nodes[30], nodes[36])
nodes[38]:insertNeighbors(nodes[35], nodes[30], nodes[37], nodes[36])
nodes[39]:insertNeighbors(nodes[47], nodes[31])
nodes[40]:insertNeighbors(nodes[48], nodes[33], nodes[49], nodes[32])
nodes[41]:insertNeighbors(nodes[34], nodes[50])
nodes[42]:insertNeighbors(nodes[43], nodes[35])
nodes[43]:insertNeighbors(nodes[42], nodes[51], nodes[35])
nodes[44]:insertNeighbors(nodes[52], nodes[46], nodes[36])
nodes[45]:insertNeighbors(nodes[36], nodes[53], nodes[54], nodes[46])
nodes[46]:insertNeighbors(nodes[44], nodes[45], nodes[54], nodes[36])
nodes[47]:insertNeighbors(nodes[55], nodes[39])
nodes[48]:insertNeighbors(nodes[56], nodes[57], nodes[58], nodes[40], nodes[59])
nodes[49]:insertNeighbors(nodes[40])
nodes[50]:insertNeighbors(nodes[60], nodes[41])
nodes[51]:insertNeighbors(nodes[61], nodes[62], nodes[43])
nodes[52]:insertNeighbors(nodes[44], nodes[63])
nodes[53]:insertNeighbors(nodes[45], nodes[64])
nodes[54]:insertNeighbors(nodes[46], nodes[45])
nodes[55]:insertNeighbors(nodes[65], nodes[66], nodes[60], nodes[47], nodes[67])
nodes[56]:insertNeighbors(nodes[57], nodes[48], nodes[68], nodes[58])
nodes[57]:insertNeighbors(nodes[48], nodes[56])
nodes[58]:insertNeighbors(nodes[69], nodes[68], nodes[48], nodes[70], nodes[56])
nodes[59]:insertNeighbors(nodes[48], nodes[71])
nodes[60]:insertNeighbors(nodes[55], nodes[50], nodes[65])
nodes[61]:insertNeighbors(nodes[72], nodes[51], nodes[62], nodes[73])
nodes[62]:insertNeighbors(nodes[61], nodes[51], nodes[73])
nodes[63]:insertNeighbors(nodes[74], nodes[52])
nodes[64]:insertNeighbors(nodes[53], nodes[70])
nodes[65]:insertNeighbors(nodes[60], nodes[55])
nodes[66]:insertNeighbors(nodes[55], nodes[75])
nodes[67]:insertNeighbors(nodes[55], nodes[76], nodes[77])
nodes[68]:insertNeighbors(nodes[58], nodes[78], nodes[69], nodes[56])
nodes[69]:insertNeighbors(nodes[58], nodes[78], nodes[68], nodes[70])
nodes[70]:insertNeighbors(nodes[78], nodes[79], nodes[64], nodes[58], nodes[80], nodes[69])
nodes[71]:insertNeighbors(nodes[59], nodes[81])
nodes[72]:insertNeighbors(nodes[61], nodes[82], nodes[83], nodes[84])
nodes[73]:insertNeighbors(nodes[62], nodes[61])
nodes[74]:insertNeighbors(nodes[85], nodes[63])
nodes[75]:insertNeighbors(nodes[66], nodes[86])
nodes[76]:insertNeighbors(nodes[67], nodes[77])
nodes[77]:insertNeighbors(nodes[87], nodes[76], nodes[67])
nodes[78]:insertNeighbors(nodes[70], nodes[69], nodes[68])
nodes[79]:insertNeighbors(nodes[70], nodes[80])
nodes[80]:insertNeighbors(nodes[79], nodes[70], nodes[88])
nodes[81]:insertNeighbors(nodes[71], nodes[89])
nodes[82]:insertNeighbors(nodes[90], nodes[83], nodes[72])
nodes[83]:insertNeighbors(nodes[72], nodes[91], nodes[82], nodes[84], nodes[92])
nodes[84]:insertNeighbors(nodes[72], nodes[83], nodes[91], nodes[93], nodes[94])
nodes[85]:insertNeighbors(nodes[74], nodes[95])
nodes[86]:insertNeighbors(nodes[75], nodes[96])
nodes[87]:insertNeighbors(nodes[77], nodes[97], nodes[98])
nodes[88]:insertNeighbors(nodes[99], nodes[80], nodes[100])
nodes[89]:insertNeighbors(nodes[81], nodes[101])
nodes[90]:insertNeighbors(nodes[95], nodes[102], nodes[82])
nodes[91]:insertNeighbors(nodes[92], nodes[103], nodes[83], nodes[84])
nodes[92]:insertNeighbors(nodes[103], nodes[104], nodes[83], nodes[91])
nodes[93]:insertNeighbors(nodes[94], nodes[98], nodes[84])
nodes[94]:insertNeighbors(nodes[93], nodes[84])
nodes[95]:insertNeighbors(nodes[85], nodes[90], nodes[102])
nodes[96]:insertNeighbors(nodes[86], nodes[105])
nodes[97]:insertNeighbors(nodes[87], nodes[98])
nodes[98]:insertNeighbors(nodes[97], nodes[87], nodes[93])
nodes[99]:insertNeighbors(nodes[106], nodes[88], nodes[100], nodes[107])
nodes[100]:insertNeighbors(nodes[99], nodes[106], nodes[108], nodes[88], nodes[107])
nodes[101]:insertNeighbors(nodes[89], nodes[109])
nodes[102]:insertNeighbors(nodes[95], nodes[90], nodes[110])
nodes[103]:insertNeighbors(nodes[91], nodes[92])
nodes[104]:insertNeighbors(nodes[111], nodes[92], nodes[112])
nodes[105]:insertNeighbors(nodes[96], nodes[113])
nodes[106]:insertNeighbors(nodes[99], nodes[114], nodes[100], nodes[115], nodes[107], nodes[116])
nodes[107]:insertNeighbors(nodes[99], nodes[115], nodes[100], nodes[106])
nodes[108]:insertNeighbors(nodes[100], nodes[117])
nodes[109]:insertNeighbors(nodes[101], nodes[118])
nodes[110]:insertNeighbors(nodes[102], nodes[119], nodes[120], nodes[121])
nodes[111]:insertNeighbors(nodes[122], nodes[104], nodes[123], nodes[124], nodes[112], nodes[125])
nodes[112]:insertNeighbors(nodes[126], nodes[111], nodes[123], nodes[104], nodes[127], nodes[124])
nodes[113]:insertNeighbors(nodes[105], nodes[128])
nodes[114]:insertNeighbors(nodes[129], nodes[106], nodes[116], nodes[130], nodes[115])
nodes[115]:insertNeighbors(nodes[129], nodes[114], nodes[116], nodes[130], nodes[107], nodes[106])
nodes[116]:insertNeighbors(nodes[106], nodes[114], nodes[115])
nodes[117]:insertNeighbors(nodes[108], nodes[131], nodes[132])
nodes[118]:insertNeighbors(nodes[109], nodes[130], nodes[133], nodes[134])
nodes[119]:insertNeighbors(nodes[110], nodes[120], nodes[121], nodes[135])
nodes[120]:insertNeighbors(nodes[110], nodes[119], nodes[136], nodes[121])
nodes[121]:insertNeighbors(nodes[110], nodes[119], nodes[120])
nodes[122]:insertNeighbors(nodes[137], nodes[111])
nodes[123]:insertNeighbors(nodes[125], nodes[124], nodes[112], nodes[111])
nodes[124]:insertNeighbors(nodes[138], nodes[112], nodes[139], nodes[111], nodes[125], nodes[140], nodes[141], nodes[123])
nodes[125]:insertNeighbors(nodes[111], nodes[124], nodes[123])
nodes[126]:insertNeighbors(nodes[142], nodes[112], nodes[127])
nodes[127]:insertNeighbors(nodes[112], nodes[126], nodes[143], nodes[142], nodes[144], nodes[145])
nodes[128]:insertNeighbors(nodes[146], nodes[147], nodes[137], nodes[113])
nodes[129]:insertNeighbors(nodes[134], nodes[114], nodes[133], nodes[130], nodes[115])
nodes[130]:insertNeighbors(nodes[129], nodes[114], nodes[133], nodes[115], nodes[134], nodes[118])
nodes[131]:insertNeighbors(nodes[132], nodes[148], nodes[149], nodes[150], nodes[151], nodes[117], nodes[152])
nodes[132]:insertNeighbors(nodes[117], nodes[131])
nodes[133]:insertNeighbors(nodes[118], nodes[130], nodes[129], nodes[134])
nodes[134]:insertNeighbors(nodes[118], nodes[130], nodes[133], nodes[129])
nodes[135]:insertNeighbors(nodes[153], nodes[119])
nodes[136]:insertNeighbors(nodes[120], nodes[154])
nodes[137]:insertNeighbors(nodes[128], nodes[147], nodes[122])
nodes[138]:insertNeighbors(nodes[155], nodes[124], nodes[141], nodes[156])
nodes[139]:insertNeighbors(nodes[124], nodes[157], nodes[158])
nodes[140]:insertNeighbors(nodes[124], nodes[159])
nodes[141]:insertNeighbors(nodes[138], nodes[124], nodes[156])
nodes[142]:insertNeighbors(nodes[126], nodes[127])
nodes[143]:insertNeighbors(nodes[144], nodes[127], nodes[145], nodes[160])
nodes[144]:insertNeighbors(nodes[161], nodes[145], nodes[143], nodes[127], nodes[162], nodes[163], nodes[160])
nodes[145]:insertNeighbors(nodes[144], nodes[127], nodes[143], nodes[160])
nodes[146]:insertNeighbors(nodes[128], nodes[164])
nodes[147]:insertNeighbors(nodes[128], nodes[137])
nodes[148]:insertNeighbors(nodes[152], nodes[131], nodes[165])
nodes[149]:insertNeighbors(nodes[150], nodes[166], nodes[131], nodes[167], nodes[168], nodes[151])
nodes[150]:insertNeighbors(nodes[151], nodes[149], nodes[131])
nodes[151]:insertNeighbors(nodes[150], nodes[167], nodes[131], nodes[149])
nodes[152]:insertNeighbors(nodes[165], nodes[131], nodes[148])
nodes[153]:insertNeighbors(nodes[135], nodes[169])
nodes[154]:insertNeighbors(nodes[136], nodes[170])
nodes[155]:insertNeighbors(nodes[138], nodes[156], nodes[171], nodes[172])
nodes[156]:insertNeighbors(nodes[138], nodes[172], nodes[141], nodes[155])
nodes[157]:insertNeighbors(nodes[158], nodes[139])
nodes[158]:insertNeighbors(nodes[139], nodes[173], nodes[157], nodes[174])
nodes[159]:insertNeighbors(nodes[140])
nodes[160]:insertNeighbors(nodes[145], nodes[143], nodes[144], nodes[161], nodes[163])
nodes[161]:insertNeighbors(nodes[175], nodes[176], nodes[177], nodes[163], nodes[160], nodes[144], nodes[178], nodes[162])
nodes[162]:insertNeighbors(nodes[177], nodes[144], nodes[163], nodes[161])
nodes[163]:insertNeighbors(nodes[161], nodes[144], nodes[162], nodes[160])
nodes[164]:insertNeighbors(nodes[146], nodes[179])
nodes[165]:insertNeighbors(nodes[152], nodes[170], nodes[148])
nodes[166]:insertNeighbors(nodes[180], nodes[181], nodes[167], nodes[149], nodes[168])
nodes[167]:insertNeighbors(nodes[151], nodes[149], nodes[168], nodes[166])
nodes[168]:insertNeighbors(nodes[180], nodes[181], nodes[167], nodes[149], nodes[166])
nodes[169]:insertNeighbors(nodes[153], nodes[182])
nodes[170]:insertNeighbors(nodes[154], nodes[165])
nodes[171]:insertNeighbors(nodes[155], nodes[172], nodes[183], nodes[184])
nodes[172]:insertNeighbors(nodes[155], nodes[156], nodes[171])
nodes[173]:insertNeighbors(nodes[174], nodes[185], nodes[158])
nodes[174]:insertNeighbors(nodes[158], nodes[179], nodes[185], nodes[173])
nodes[175]:insertNeighbors(nodes[178], nodes[186], nodes[183], nodes[161])
nodes[176]:insertNeighbors(nodes[161], nodes[177], nodes[182])
nodes[177]:insertNeighbors(nodes[187], nodes[188], nodes[161], nodes[176], nodes[162], nodes[178], nodes[189])
nodes[178]:insertNeighbors(nodes[175], nodes[177], nodes[161])
nodes[179]:insertNeighbors(nodes[164], nodes[174])
nodes[180]:insertNeighbors(nodes[166], nodes[181], nodes[190], nodes[168])
nodes[181]:insertNeighbors(nodes[191], nodes[190], nodes[192], nodes[166], nodes[168], nodes[180])
nodes[182]:insertNeighbors(nodes[169], nodes[176])
nodes[183]:insertNeighbors(nodes[184], nodes[186], nodes[175], nodes[171])
nodes[184]:insertNeighbors(nodes[171], nodes[183])
nodes[185]:insertNeighbors(nodes[174], nodes[173])
nodes[186]:insertNeighbors(nodes[183], nodes[175])
nodes[187]:insertNeighbors(nodes[193], nodes[189], nodes[188], nodes[177])
nodes[188]:insertNeighbors(nodes[187], nodes[189], nodes[177])
nodes[189]:insertNeighbors(nodes[187], nodes[188], nodes[177])
nodes[190]:insertNeighbors(nodes[180], nodes[192], nodes[191], nodes[181])
nodes[191]:insertNeighbors(nodes[190], nodes[192], nodes[194], nodes[195], nodes[181])
nodes[192]:insertNeighbors(nodes[190], nodes[195], nodes[194], nodes[191], nodes[181])
nodes[193]:insertNeighbors(nodes[187], nodes[196])
nodes[194]:insertNeighbors(nodes[197], nodes[192], nodes[195], nodes[191])
nodes[195]:insertNeighbors(nodes[198], nodes[191], nodes[192], nodes[194], nodes[199], nodes[197])
nodes[196]:insertNeighbors(nodes[193], nodes[200])
nodes[197]:insertNeighbors(nodes[195], nodes[199], nodes[198], nodes[194])
nodes[198]:insertNeighbors(nodes[201], nodes[202], nodes[197], nodes[195], nodes[199])
nodes[199]:insertNeighbors(nodes[198], nodes[201], nodes[202], nodes[197], nodes[195])
nodes[200]:insertNeighbors(nodes[203], nodes[204], nodes[205], nodes[196])
nodes[201]:insertNeighbors(nodes[202], nodes[206], nodes[198], nodes[199])
nodes[202]:insertNeighbors(nodes[198], nodes[206], nodes[207], nodes[208], nodes[199], nodes[201])
nodes[203]:insertNeighbors(nodes[205], nodes[209], nodes[200], nodes[210])
nodes[204]:insertNeighbors(nodes[200], nodes[205])
nodes[205]:insertNeighbors(nodes[203], nodes[204], nodes[200], nodes[210])
nodes[206]:insertNeighbors(nodes[201], nodes[208], nodes[202], nodes[207])
nodes[207]:insertNeighbors(nodes[211], nodes[206], nodes[202], nodes[208], nodes[212])
nodes[208]:insertNeighbors(nodes[211], nodes[206], nodes[202], nodes[212], nodes[207])
nodes[209]:insertNeighbors(nodes[203], nodes[213], nodes[214], nodes[210])
nodes[210]:insertNeighbors(nodes[203], nodes[209], nodes[214], nodes[205])
nodes[211]:insertNeighbors(nodes[215], nodes[216], nodes[208], nodes[207], nodes[212])
nodes[212]:insertNeighbors(nodes[211], nodes[215], nodes[207], nodes[208])
nodes[213]:insertNeighbors(nodes[217], nodes[209], nodes[218], nodes[214])
nodes[214]:insertNeighbors(nodes[217], nodes[209], nodes[210], nodes[213])
nodes[215]:insertNeighbors(nodes[212], nodes[216], nodes[218], nodes[211])
nodes[216]:insertNeighbors(nodes[211], nodes[215], nodes[218], nodes[217])
nodes[217]:insertNeighbors(nodes[216], nodes[213], nodes[218], nodes[214])
nodes[218]:insertNeighbors(nodes[216], nodes[213], nodes[217], nodes[215])

return nodes