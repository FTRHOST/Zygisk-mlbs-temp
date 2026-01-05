#pragma once

std::string RankToString(int uiRankLevel){
	std::string strRank;
	switch(uiRankLevel){
		case 1:
			strRank += "Warrior III * 0";
			break;
		case 2:
			strRank += "Warrior III * 1";
			break;
		case 3:
			strRank += "Warrior III * 2";
			break;
		case 4:
			strRank += "Warrior III * 3";
			break;
		case 5:
			strRank += "Warrior II * 1";
			break;
		case 6:
			strRank += "Warrior II * 2";
			break;
		case 7:
			strRank += "Warrior II * 3";
			break;
		case 8:
			strRank += "Warrior I * 1";
			break;
		case 9:
			strRank += "Warrior I * 2";
			break;
		case 10:
			strRank += "Warrior I * 3";
			break;
		case 11:
			strRank += "Elite III * 0";
			break;
		case 12:
			strRank += "Elite III * 1";
			break;
		case 13:
			strRank += "Elite III * 2";
			break;
		case 14:
			strRank += "Elite III * 3";
			break;
		case 15:
			strRank += "Elite III * 4";
			break;
		case 16:
			strRank += "Elite II * 0";
			break;
		case 17:
			strRank += "Elite II * 1";
			break;
		case 18:
			strRank += "Elite II * 2";
			break;
		case 19:
			strRank += "Elite II * 3";
			break;
		case 20:
			strRank += "Elite II * 4";
			break;
		case 21:
			strRank += "Elite I * 0";
			break;
		case 22:
			strRank += "Elite I * 1";
			break;
		case 23:
			strRank += "Elite I * 2";
			break;
		case 24:
			strRank += "Elite I * 3";
			break;
		case 25:
			strRank += "Elite I * 4";
			break;
		case 26:
			strRank += "Master IV * 0";
			break;
		case 27:
			strRank += "Master IV * 1";
			break;
		case 28:
			strRank += "Master IV * 2";
			break;
		case 29:
			strRank += "Master IV * 3";
			break;
		case 30:
			strRank += "Master IV * 4";
			break;
		case 31:
			strRank += "Master III * 0";
			break;
		case 32:
			strRank += "Master III * 1";
			break;
		case 33:
			strRank += "Master III * 2";
			break;
		case 34:
			strRank += "Master III * 3";
			break;
		case 35:
			strRank += "Master III * 4";
			break;
		case 36:
			strRank += "Master II * 0";
			break;
		case 37:
			strRank += "Master II * 1";
			break;
		case 38:
			strRank += "Master II * 2";
			break;
		case 39:
			strRank += "Master II * 3";
			break;
		case 40:
			strRank += "Master II * 4";
			break;
		case 41:
			strRank += "Master I * 0";
			break;
		case 42:
			strRank += "Master I * 1";
			break;
		case 43:
			strRank += "Master I * 2";
			break;
		case 44:
			strRank += "Master I * 3";
			break;
		case 45:
			strRank += "Master I * 4";
			break;
		case 46:
			strRank += "Grandmaster V * 0";
			break;
		case 47:
			strRank += "Grandmaster V * 1";
			break;
		case 48:
			strRank += "Grandmaster V * 2";
			break;
		case 49:
			strRank += "Grandmaster V * 3";
			break;
		case 50:
			strRank += "Grandmaster V * 4";
			break;
		case 51:
			strRank += "Grandmaster V * 5";
			break;
		case 52:
			strRank += "Grandmaster IV * 0";
			break;
		case 53:
			strRank += "Grandmaster IV * 1";
			break;
		case 54:
			strRank += "Grandmaster IV * 2";
			break;
		case 55:
			strRank += "Grandmaster IV * 3";
			break;
		case 56:
			strRank += "Grandmaster IV * 4";
			break;
		case 57:
			strRank += "Grandmaster IV * 5";
			break;
		case 58:
			strRank += "Grandmaster III * 0";
			break;
		case 59:
			strRank += "Grandmaster III * 1";
			break;
		case 60:
			strRank += "Grandmaster III * 2";
			break;
		case 61:
			strRank += "Grandmaster III * 3";
			break;
		case 62:
			strRank += "Grandmaster III * 4";
			break;
		case 63:
			strRank += "Grandmaster III * 5";
			break;
		case 64:
			strRank += "Grandmaster II * 0";
			break;
		case 65:
			strRank += "Grandmaster II * 1";
			break;
		case 66:
			strRank += "Grandmaster II * 2";
			break;
		case 67:
			strRank += "Grandmaster II * 3";
			break;
		case 68:
			strRank += "Grandmaster II * 4";
			break;
		case 69:
			strRank += "Grandmaster II * 5";
			break;
		case 70:
			strRank += "Grandmaster I * 0";
			break;
		case 71:
			strRank += "Grandmaster I * 1";
			break;
		case 72:
			strRank += "Grandmaster I * 2";
			break;
		case 73:
			strRank += "Grandmaster I * 3";
			break;
		case 74:
			strRank += "Grandmaster I * 4";
			break;
		case 75:
			strRank += "Grandmaster I * 5";
			break;
		case 76:
			strRank += "Epic V * 0";
			break;
		case 77:
			strRank += "Epic V * 1";
			break;
		case 78:
			strRank += "Epic V * 2";
			break;
		case 79:
			strRank += "Epic V * 3";
			break;
		case 80:
			strRank += "Epic V * 4";
			break;
		case 81:
			strRank += "Epic V * 5";
			break;
		case 82:
			strRank += "Epic IV * 0";
			break;
		case 83:
			strRank += "Epic IV * 1";
			break;
		case 84:
			strRank += "Epic IV * 2";
			break;
		case 85:
			strRank += "Epic IV * 3";
			break;
		case 86:
			strRank += "Epic IV * 4";
			break;
		case 87:
			strRank += "Epic IV * 5";
			break;
		case 88:
			strRank += "Epic III * 0";
			break;
		case 89:
			strRank += "Epic III * 1";
			break;
		case 90:
			strRank += "Epic III * 2";
			break;
		case 91:
			strRank += "Epic III * 3";
			break;
		case 92:
			strRank += "Epic III * 4";
			break;
		case 93:
			strRank += "Epic III * 5";
			break;
		case 94:
			strRank += "Epic II * 0";
			break;
		case 95:
			strRank += "Epic II * 1";
			break;
		case 96:
			strRank += "Epic II * 2";
			break;
		case 97:
			strRank += "Epic II * 3";
			break;
		case 98:
			strRank += "Epic II * 4";
			break;
		case 99:
			strRank += "Epic II * 5";
			break;
		case 100:
			strRank += "Epic I * 0";
			break;
		case 101:
			strRank += "Epic I * 1";
			break;
		case 102:
			strRank += "Epic I * 2";
			break;
		case 103:
			strRank += "Epic I * 3";
			break;
		case 104:
			strRank += "Epic I * 4";
			break;
		case 105:
			strRank += "Epic I * 5";
			break;
		case 106:
			strRank += "Legend V * 0";
			break;
		case 107:
			strRank += "Legend V * 1";
			break;
		case 108:
			strRank += "Legend V * 2";
			break;
		case 109:
			strRank += "Legend V * 3";
			break;
		case 110:
			strRank += "Legend V * 4";
			break;
		case 111:
			strRank += "Legend V * 5";
			break;
		case 112:
			strRank += "Legend IV * 0";
			break;
		case 113:
			strRank += "Legend IV * 1";
			break;
		case 114:
			strRank += "Legend IV * 2";
			break;
		case 115:
			strRank += "Legend IV * 3";
			break;
		case 116:
			strRank += "Legend IV * 4";
			break;
		case 117:
			strRank += "Legend IV * 5";
			break;
		case 118:
			strRank += "Legend III * 0";
			break;
		case 119:
			strRank += "Legend III * 1";
			break;
		case 120:
			strRank += "Legend III * 2";
			break;
		case 121:
			strRank += "Legend III * 3";
			break;
		case 122:
			strRank += "Legend III * 4";
			break;
		case 123:
			strRank += "Legend III * 5";
			break;
		case 124:
			strRank += "Legend II * 0";
			break;
		case 125:
			strRank += "Legend II * 1";
			break;
		case 126:
			strRank += "Legend II * 2";
			break;
		case 127:
			strRank += "Legend II * 3";
			break;
		case 128:
			strRank += "Legend II * 4";
			break;
		case 129:
			strRank += "Legend II * 5";
			break;
		case 130:
			strRank += "Legend I * 0";
			break;
		case 131:
			strRank += "Legend I * 1";
			break;
		case 132:
			strRank += "Legend I * 2";
			break;
		case 133:
			strRank += "Legend I * 3";
			break;
		case 134:
			strRank += "Legend I * 4";
			break;
		case 135:
			strRank += "Legend I * 5";
			break;
		case 136:
			strRank += "Mythic * 0";
			break;
		case 137:
			strRank += "Mythic * 1";
			break;
		case 138:
			strRank += "Mythic * 2";
			break;
		case 139:
			strRank += "Mythic * 3";
			break;
		case 140:
			strRank += "Mythic * 4";
			break;
		case 141:
			strRank += "Mythic * 5";
			break;
		case 142:
			strRank += "Mythic * 6";
			break;
		case 143:
			strRank += "Mythic * 7";
			break;
		case 144:
			strRank += "Mythic * 8";
			break;
		case 145:
			strRank += "Mythic * 9";
			break;
		case 146:
			strRank += "Mythic * 10";
			break;
		case 147:
			strRank += "Mythic * 11";
			break;
		case 148:
			strRank += "Mythic * 12";
			break;
		case 149:
			strRank += "Mythic * 13";
			break;
		case 150:
			strRank += "Mythic * 14";
			break;
		case 151:
			strRank += "Mythic * 15";
			break;
		case 152:
			strRank += "Mythic * 16";
			break;
		case 153:
			strRank += "Mythic * 17";
			break;
		case 154:
			strRank += "Mythic * 18";
			break;
		case 155:
			strRank += "Mythic * 19";
			break;
		case 156:
			strRank += "Mythic * 20";
			break;
		case 157:
			strRank += "Mythic * 21";
			break;
		case 158:
			strRank += "Mythic * 22";
			break;
		case 159:
			strRank += "Mythic * 23";
			break;
		case 160:
			strRank += "Mythic * 24";
			break;
		case 161:
			strRank += "Mythic * 25";
			break;
		case 162:
			strRank += "Mythic * 26";
			break;
		case 163:
			strRank += "Mythic * 27";
			break;
		case 164:
			strRank += "Mythic * 28";
			break;
		case 165:
			strRank += "Mythic * 29";
			break;
		case 166:
			strRank += "Mythic * 30";
			break;
		case 167:
			strRank += "Mythic * 31";
			break;
		case 168:
			strRank += "Mythic * 32";
			break;
		case 169:
			strRank += "Mythic * 33";
			break;
		case 170:
			strRank += "Mythic * 34";
			break;
		case 171:
			strRank += "Mythic * 35";
			break;
		case 172:
			strRank += "Mythic * 36";
			break;
		case 173:
			strRank += "Mythic * 37";
			break;
		case 174:
			strRank += "Mythic * 38";
			break;
		case 175:
			strRank += "Mythic * 39";
			break;
		case 176:
			strRank += "Mythic * 40";
			break;
		case 177:
			strRank += "Mythic * 41";
			break;
		case 178:
			strRank += "Mythic * 42";
			break;
		case 179:
			strRank += "Mythic * 43";
			break;
		case 180:
			strRank += "Mythic * 44";
			break;
		case 181:
			strRank += "Mythic * 45";
			break;
		case 182:
			strRank += "Mythic * 46";
			break;
		case 183:
			strRank += "Mythic * 47";
			break;
		case 184:
			strRank += "Mythic * 48";
			break;
		case 185:
			strRank += "Mythic * 49";
			break;
		case 186:
			strRank += "Mythic * 50";
			break;
		case 187:
			strRank += "Mythic * 51";
			break;
		case 188:
			strRank += "Mythic * 52";
			break;
		case 189:
			strRank += "Mythic * 53";
			break;
		case 190:
			strRank += "Mythic * 54";
			break;
		case 191:
			strRank += "Mythic * 55";
			break;
		case 192:
			strRank += "Mythic * 56";
			break;
		case 193:
			strRank += "Mythic * 57";
			break;
		case 194:
			strRank += "Mythic * 58";
			break;
		case 195:
			strRank += "Mythic * 59";
			break;
		case 196:
			strRank += "Mythic * 60";
			break;
		case 197:
			strRank += "Mythic * 61";
			break;
		case 198:
			strRank += "Mythic * 62";
			break;
		case 199:
			strRank += "Mythic * 63";
			break;	
		case 200:
			strRank += "Mythic * 64";
			break;	
		case 201:
			strRank += "Mythic * 65";
			break;
		case 202:
			strRank += "Mythic * 66";
			break;
		case 203:
			strRank += "Mythic * 67";
			break;
		case 204:
			strRank += "Mythic * 68";
			break;
		case 205:
			strRank += "Mythic * 69";
			break;
		case 206:
			strRank += "Mythic * 70";
			break;
		case 207:
			strRank += "Mythic * 71";
			break;
		case 208:
			strRank += "Mythic * 72";
			break;
		case 209:
			strRank += "Mythic * 73";
			break;
		case 210:
			strRank += "Mythic * 74";
			break;
		case 211:
			strRank += "Mythic * 75";
			break;
		case 212:
			strRank += "Mythic * 76";
			break;
		case 213:
			strRank += "Mythic * 77";
			break;
		case 214:
			strRank += "Mythic * 78";
			break;
		case 215:
			strRank += "Mythic * 79";
			break;
		case 216:
			strRank += "Mythic * 80";
			break;
		case 217:
			strRank += "Mythic * 81";
			break;
		case 218:
			strRank += "Mythic * 82";
			break;
		case 219:
			strRank += "Mythic * 83";
			break;
		case 220:
			strRank += "Mythic * 84";
			break;
		case 221:
			strRank += "Mythic * 85";
			break;
		case 222:
			strRank += "Mythic * 86";
			break;
		case 223:
			strRank += "Mythic * 87";
			break;
		case 224:
			strRank += "Mythic * 88";
			break;
		case 225:
			strRank += "Mythic * 89";
			break;
		case 226:
			strRank += "Mythic * 90";
			break;
		case 227:
			strRank += "Mythic * 91";
			break;
		case 228:
			strRank += "Mythic * 92";
			break;
		case 229:
			strRank += "Mythic * 93";
			break;
		case 230:
			strRank += "Mythic * 94";
			break;
		case 231:
			strRank += "Mythic * 95";
			break;
		case 232:
			strRank += "Mythic * 96";
			break;
		case 233:
			strRank += "Mythic * 97";
			break;
		case 234:
			strRank += "Mythic * 98";
			break;
		case 235:
			strRank += "Mythic * 99";
			break;
		case 236:
			strRank += "Mythic * 100";
			break;
		case 237:
			strRank += "Mythic * 101";
			break;
		case 238:
			strRank += "Mythic * 102";
			break;
		case 239:
			strRank += "Mythic * 103";
			break;
		case 240:
			strRank += "Mythic * 104";
			break;
		case 241:
			strRank += "Mythic * 105";
			break;
		case 242:
			strRank += "Mythic * 106";
			break;
		case 243:
			strRank += "Mythic * 107";
			break;
		case 244:
			strRank += "Mythic * 108";
			break;
		case 245:
			strRank += "Mythic * 109";
			break;
		case 246:
			strRank += "Mythic * 110";
			break;
		case 247:
			strRank += "Mythic * 111";
			break;
		case 248:
			strRank += "Mythic * 112";
			break;
		case 249:
			strRank += "Mythic * 113";
			break;
		case 250:
			strRank += "Mythic * 114";
			break;
		case 251:
			strRank += "Mythic * 115";
			break;
		case 252:
			strRank += "Mythic * 116";
			break;
		case 253:
			strRank += "Mythic * 117";
			break;
		case 254:
			strRank += "Mythic * 118";
			break;
		case 255:
			strRank += "Mythic * 119";
			break;
		case 256:
			strRank += "Mythic * 120";
			break;
		case 257:
			strRank += "Mythic * 121";
			break;
		case 258:
			strRank += "Mythic * 122";
			break;
		case 259:
			strRank += "Mythic * 123";
			break;
		case 260:
			strRank += "Mythic * 124";
			break;
		case 261:
			strRank += "Mythic * 125";
			break;
		case 262:
			strRank += "Mythic * 126";
			break;
		case 263:
			strRank += "Mythic * 127";
			break;
		case 264:
			strRank += "Mythic * 128";
			break;
		case 265:
			strRank += "Mythic * 129";
			break;
		case 266:
			strRank += "Mythic * 130";
			break;
		case 267:
			strRank += "Mythic * 131";
			break;
		case 268:
			strRank += "Mythic * 132";
			break;
		case 269:
			strRank += "Mythic * 133";
			break;
		case 270:
			strRank += "Mythic * 134";
			break;
		case 271:
			strRank += "Mythic * 135";
			break;
		case 272:
			strRank += "Mythic * 136";
			break;
		case 273:
			strRank += "Mythic * 137";
			break;
		case 274:
			strRank += "Mythic * 138";
			break;
		case 275:
			strRank += "Mythic * 139";
			break;
		case 276:
			strRank += "Mythic * 140";
			break;
		case 277:
			strRank += "Mythic * 141";
			break;
		case 278:
			strRank += "Mythic * 142";
			break;
		case 279:
			strRank += "Mythic * 143";
			break;
		case 280:
			strRank += "Mythic * 144";
			break;
		case 281:
			strRank += "Mythic * 145";
			break;
		case 282:
			strRank += "Mythic * 146";
			break;
		case 283:
			strRank += "Mythic * 147";
			break;
		case 284:
			strRank += "Mythic * 148";
			break;
		case 285:
			strRank += "Mythic * 149";
			break;
		case 286:
			strRank += "Mythic * 150";
			break;
		case 287:
			strRank += "Mythic * 151";
			break;
		case 288:
			strRank += "Mythic * 152";
			break;
		case 289:
			strRank += "Mythic * 153";
			break;
		case 290:
			strRank += "Mythic * 154";
			break;
		case 291:
			strRank += "Mythic * 155";
			break;
		case 292:
			strRank += "Mythic * 156";
			break;
		case 293:
			strRank += "Mythic * 157";
			break;
		case 294:
			strRank += "Mythic * 158";
			break;
		case 295:
			strRank += "Mythic * 159";
			break;
		case 296:
			strRank += "Mythic * 160";
			break;
		case 297:
			strRank += "Mythic * 161";
			break;
		case 298:
			strRank += "Mythic * 162";
			break;
		case 299:
			strRank += "Mythic * 163";
			break;
		case 300:
			strRank += "Mythic * 164";
			break;
		case 301:
			strRank += "Mythic * 165";
			break;
		case 302:
			strRank += "Mythic * 166";
			break;
		case 303:
			strRank += "Mythic * 167";
			break;
		case 304:
			strRank += "Mythic * 168";
			break;
		case 305:
			strRank += "Mythic * 169";
			break;
		case 306:
			strRank += "Mythic * 170";
			break;
		case 307:
			strRank += "Mythic * 171";
			break;
		case 308:
			strRank += "Mythic * 172";
			break;
		case 309:
			strRank += "Mythic * 173";
			break;
		case 310:
			strRank += "Mythic * 174";
			break;
		case 311:
			strRank += "Mythic * 175";
			break;
		case 312:
			strRank += "Mythic * 176";
			break;
		case 313:
			strRank += "Mythic * 177";
			break;
		case 314:
			strRank += "Mythic * 178";
			break;
		case 315:
			strRank += "Mythic * 179";
			break;
		case 316:
			strRank += "Mythic * 180";
			break;
		case 317:
			strRank += "Mythic * 181";
			break;
		case 318:
			strRank += "Mythic * 182";
			break;
		case 319:
			strRank += "Mythic * 183";
			break;
		case 320:
			strRank += "Mythic * 184";
			break;
		case 321:
			strRank += "Mythic * 185";
			break;
		case 322:
			strRank += "Mythic * 186";
			break;
		case 323:
			strRank += "Mythic * 187";
			break;
		case 324:
			strRank += "Mythic * 188";
			break;
		case 325:
			strRank += "Mythic * 189";
			break;
		case 326:
			strRank += "Mythic * 190";
			break;
		case 327:
			strRank += "Mythic * 191";
			break;
		case 328:
			strRank += "Mythic * 192";
			break;
		case 329:
			strRank += "Mythic * 193";
			break;
		case 330:
			strRank += "Mythic * 194";
			break;
		case 331:
			strRank += "Mythic * 195";
			break;
		case 332:
			strRank += "Mythic * 196";
			break;
		case 333:
			strRank += "Mythic * 197";
			break;
		case 334:
			strRank += "Mythic * 198";
			break;
		case 335:
			strRank += "Mythic * 199";
			break;
		case 336:
			strRank += "Mythic * 200";
			break;
		case 337:
			strRank += "Mythic * 201";
			break;
		case 338:
			strRank += "Mythic * 202";
			break;
		case 339:
			strRank += "Mythic * 203";
			break;
		case 340:
			strRank += "Mythic * 204";
			break;
		case 341:
			strRank += "Mythic * 205";
			break;
		case 342:
			strRank += "Mythic * 206";
			break;
		case 343:
			strRank += "Mythic * 207";
			break;
		case 344:
			strRank += "Mythic * 208";
			break;
		case 345:
			strRank += "Mythic * 209";
			break;
		case 346:
			strRank += "Mythic * 210";
			break;
		case 347:
			strRank += "Mythic * 211";
			break;
		case 348:
			strRank += "Mythic * 212";
			break;
		case 349:
			strRank += "Mythic * 213";
			break;
		case 350:
			strRank += "Mythic * 214";
			break;
		case 351:
			strRank += "Mythic * 215";
			break;
		case 352:
			strRank += "Mythic * 216";
			break;
		case 353:
			strRank += "Mythic * 217";
			break;
		case 354:
			strRank += "Mythic * 218";
			break;
		case 355:
			strRank += "Mythic * 219";
			break;
		case 356:
			strRank += "Mythic * 220";
			break;
		case 357:
			strRank += "Mythic * 221";
			break;
		case 358:
			strRank += "Mythic * 222";
			break;
		case 359:
			strRank += "Mythic * 223";
			break;
		case 360:
			strRank += "Mythic * 224";
			break;
		case 361:
			strRank += "Mythic * 225";
			break;
		case 362:
			strRank += "Mythic * 226";
			break;
		case 363:
			strRank += "Mythic * 227";
			break;
		case 364:
			strRank += "Mythic * 228";
			break;
		case 365:
			strRank += "Mythic * 229";
			break;
		case 366:
			strRank += "Mythic * 230";
			break;
		case 367:
			strRank += "Mythic * 231";
			break;
		case 368:
			strRank += "Mythic * 232";
			break;
		case 369:
			strRank += "Mythic * 233";
			break;
		case 370:
			strRank += "Mythic * 234";
			break;
		case 371:
			strRank += "Mythic * 235";
			break;
		case 372:
			strRank += "Mythic * 236";
			break;
		case 373:
			strRank += "Mythic * 237";
			break;
		case 374:
			strRank += "Mythic * 238";
			break;
		case 375:
			strRank += "Mythic * 239";
			break;
		case 376:
			strRank += "Mythic * 240";
			break;
		case 377:
			strRank += "Mythic * 241";
			break;
		case 378:
			strRank += "Mythic * 242";
			break;
		case 379:
			strRank += "Mythic * 243";
			break;
		case 380:
			strRank += "Mythic * 244";
			break;
		case 381:
			strRank += "Mythic * 245";
			break;
		case 382:
			strRank += "Mythic * 246";
			break;
		case 383:
			strRank += "Mythic * 247";
			break;
		case 384:
			strRank += "Mythic * 248";
			break;
		case 385:
			strRank += "Mythic * 249";
			break;
		case 386:
			strRank += "Mythic * 250";
			break;
		case 387:
			strRank += "Mythic * 251";
			break;
		case 388:
			strRank += "Mythic * 252";
			break;
		case 389:
			strRank += "Mythic * 253";
			break;
		case 390:
			strRank += "Mythic * 254";
			break;
		case 391:
			strRank += "Mythic * 255";
			break;
		case 392:
			strRank += "Mythic * 256";
			break;
		case 393:
			strRank += "Mythic * 257";
			break;
		case 394:
			strRank += "Mythic * 258";
			break;
		case 395:
			strRank += "Mythic * 259";
			break;
		case 396:
			strRank += "Mythic * 260";
			break;
		case 397:
			strRank += "Mythic * 261";
			break;
		case 398:
			strRank += "Mythic * 262";
			break;
		case 399:
			strRank += "Mythic * 263";
			break;
		case 400:
			strRank += "Mythic * 264";
			break;
		case 401:
			strRank += "Mythic * 265";
			break;
		case 402:
			strRank += "Mythic * 266";
			break;
		case 403:
			strRank += "Mythic * 267";
			break;
		case 404:
			strRank += "Mythic * 268";
			break;
		case 405:
			strRank += "Mythic * 269";
			break;
		case 406:
			strRank += "Mythic * 270";
			break;
		case 407:
			strRank += "Mythic * 271";
			break;
		case 408:
			strRank += "Mythic * 272";
			break;
		case 409:
			strRank += "Mythic * 273";
			break;
		case 410:
			strRank += "Mythic * 274";
			break;
		case 411:
			strRank += "Mythic * 275";
			break;
		case 412:
			strRank += "Mythic * 276";
			break;
		case 413:
			strRank += "Mythic * 277";
			break;
		case 414:
			strRank += "Mythic * 278";
			break;
		case 415:
			strRank += "Mythic * 279";
			break;
		case 416:
			strRank += "Mythic * 280";
			break;
		case 417:
			strRank += "Mythic * 281";
			break;
		case 418:
			strRank += "Mythic * 282";
			break;
		case 419:
			strRank += "Mythic * 283";
			break;
		case 420:
			strRank += "Mythic * 284";
			break;
		case 421:
			strRank += "Mythic * 285";
			break;
		case 422:
			strRank += "Mythic * 286";
			break;
		case 423:
			strRank += "Mythic * 287";
			break;
		case 424:
			strRank += "Mythic * 288";
			break;
		case 425:
			strRank += "Mythic * 289";
			break;
		case 426:
			strRank += "Mythic * 290";
			break;
		case 427:
			strRank += "Mythic * 291";
			break;
		case 428:
			strRank += "Mythic * 292";
			break;
		case 429:
			strRank += "Mythic * 293";
			break;
		case 430:
			strRank += "Mythic * 294";
			break;
		case 431:
			strRank += "Mythic * 295";
			break;
		case 432:
			strRank += "Mythic * 296";
			break;
		case 433:
			strRank += "Mythic * 297";
			break;
		case 434:
			strRank += "Mythic * 298";
			break;
		case 435:
			strRank += "Mythic * 299";
			break;
		case 436:
			strRank += "Mythic * 300";
			break;
		case 437:
			strRank += "Mythic * 301";
			break;
		case 438:
			strRank += "Mythic * 302";
			break;
		case 439:
			strRank += "Mythic * 303";
			break;
		case 440:
			strRank += "Mythic * 304";
			break;
		case 441:
			strRank += "Mythic * 305";
			break;
		case 442:
			strRank += "Mythic * 306";
			break;
		case 443:
			strRank += "Mythic * 307";
			break;
		case 444:
			strRank += "Mythic * 308";
			break;
		case 445:
			strRank += "Mythic * 309";
			break;
		case 446:
			strRank += "Mythic * 310";
			break;
		case 447:
			strRank += "Mythic * 311";
			break;
		case 448:
			strRank += "Mythic * 312";
			break;
		case 449:
			strRank += "Mythic * 313";
			break;
		case 450:
			strRank += "Mythic * 314";
			break;
		case 451:
			strRank += "Mythic * 315";
			break;
		case 452:
			strRank += "Mythic * 316";
			break;
		case 453:
			strRank += "Mythic * 317";
			break;
		case 454:
			strRank += "Mythic * 318";
			break;
		case 455:
			strRank += "Mythic * 319";
			break;
		case 456:
			strRank += "Mythic * 320";
			break;
		case 457:
			strRank += "Mythic * 321";
			break;
		case 458:
			strRank += "Mythic * 322";
			break;
		case 459:
			strRank += "Mythic * 323";
			break;
		case 460:
			strRank += "Mythic * 324";
			break;
		case 461:
			strRank += "Mythic * 325";
			break;
		case 462:
			strRank += "Mythic * 326";
			break;
		case 463:
			strRank += "Mythic * 327";
			break;
		case 464:
			strRank += "Mythic * 328";
			break;
		case 465:
			strRank += "Mythic * 329";
			break;
		case 466:
			strRank += "Mythic * 330";
			break;
		case 467:
			strRank += "Mythic * 331";
			break;
		case 468:
			strRank += "Mythic * 332";
			break;
		case 469:
			strRank += "Mythic * 333";
			break;
		case 470:
			strRank += "Mythic * 334";
			break;
		case 471:
			strRank += "Mythic * 335";
			break;
		case 472:
			strRank += "Mythic * 336";
			break;
		case 473:
			strRank += "Mythic * 337";
			break;
		case 474:
			strRank += "Mythic * 338";
			break;
		case 475:
			strRank += "Mythic * 339";
			break;
		case 476:
			strRank += "Mythic * 340";
			break;
		case 477:
			strRank += "Mythic * 341";
			break;
		case 478:
			strRank += "Mythic * 342";
			break;
		case 479:
			strRank += "Mythic * 343";
			break;
		case 480:
			strRank += "Mythic * 344";
			break;
		case 481:
			strRank += "Mythic * 345";
			break;
		case 482:
			strRank += "Mythic * 346";
			break;
		case 483:
			strRank += "Mythic * 347";
			break;
		case 484:
			strRank += "Mythic * 348";
			break;
		case 485:
			strRank += "Mythic * 349";
			break;
		case 486:
			strRank += "Mythic * 350";
			break;
		case 487:
			strRank += "Mythic * 351";
			break;
		case 488:
			strRank += "Mythic * 352";
			break;
		case 489:
			strRank += "Mythic * 353";
			break;
		case 490:
			strRank += "Mythic * 354";
			break;
		case 491:
			strRank += "Mythic * 355";
			break;
		case 492:
			strRank += "Mythic * 356";
			break;
		case 493:
			strRank += "Mythic * 357";
			break;
		case 494:
			strRank += "Mythic * 358";
			break;
		case 495:
			strRank += "Mythic * 359";
			break;
		case 496:
			strRank += "Mythic * 360";
			break;
		case 497:
			strRank += "Mythic * 361";
			break;
		case 498:
			strRank += "Mythic * 362";
			break;
		case 499:
			strRank += "Mythic * 363";
			break;
		case 500:
			strRank += "Mythic * 364";
			break;
		case 501:
			strRank += "Mythic * 365";
			break;
		case 502:
			strRank += "Mythic * 366";
			break;
		case 503:
			strRank += "Mythic * 367";
			break;
		case 504:
			strRank += "Mythic * 368";
			break;
		case 505:
			strRank += "Mythic * 369";
			break;
		case 506:
			strRank += "Mythic * 370";
			break;
		case 507:
			strRank += "Mythic * 371";
			break;
		case 508:
			strRank += "Mythic * 372";
			break;
		case 509:
			strRank += "Mythic * 373";
			break;
		case 510:
			strRank += "Mythic * 374";
			break;
		case 511:
			strRank += "Mythic * 375";
			break;
		case 512:
			strRank += "Mythic * 376";
			break;
		case 513:
			strRank += "Mythic * 377";
			break;
		case 514:
			strRank += "Mythic * 378";
			break;
		case 515:
			strRank += "Mythic * 379";
			break;
		case 516:
			strRank += "Mythic * 380";
			break;
		case 517:
			strRank += "Mythic * 381";
			break;
		case 518:
			strRank += "Mythic * 382";
			break;
		case 519:
			strRank += "Mythic * 383";
			break;
		case 520:
			strRank += "Mythic * 384";
			break;
		case 521:
			strRank += "Mythic * 385";
			break;
		case 522:
			strRank += "Mythic * 386";
			break;
		case 523:
			strRank += "Mythic * 387";
			break;
		case 524:
			strRank += "Mythic * 388";
			break;
		case 525:
			strRank += "Mythic * 389";
			break;
		case 526:
			strRank += "Mythic * 390";
			break;
		case 527:
			strRank += "Mythic * 391";
			break;
		case 528:
			strRank += "Mythic * 392";
			break;
		case 529:
			strRank += "Mythic * 393";
			break;
		case 530:
			strRank += "Mythic * 394";
			break;
		case 531:
			strRank += "Mythic * 395";
			break;
		case 532:
			strRank += "Mythic * 396";
			break;
		case 533:
			strRank += "Mythic * 397";
			break;
		case 534:
			strRank += "Mythic * 398";
			break;
		case 535:
			strRank += "Mythic * 399";
			break;
		case 536:
			strRank += "Mythic * 400";
			break;
		case 537:
			strRank += "Mythic * 401";
			break;
		case 538:
			strRank += "Mythic * 402";
			break;
		case 539:
			strRank += "Mythic * 403";
			break;
		case 540:
			strRank += "Mythic * 404";
			break;
		case 541:
			strRank += "Mythic * 405";
			break;
		case 542:
			strRank += "Mythic * 406";
			break;
		case 543:
			strRank += "Mythic * 407";
			break;
		case 544:
			strRank += "Mythic * 408";
			break;
		case 545:
			strRank += "Mythic * 409";
			break;
		case 546:
			strRank += "Mythic * 410";
			break;
		case 547:
			strRank += "Mythic * 411";
			break;
		case 548:
			strRank += "Mythic * 412";
			break;
		case 549:
			strRank += "Mythic * 413";
			break;
		case 550:
			strRank += "Mythic * 414";
			break;
		case 551:
			strRank += "Mythic * 415";
			break;
		case 552:
			strRank += "Mythic * 416";
			break;
		case 553:
			strRank += "Mythic * 417";
			break;
		case 554:
			strRank += "Mythic * 418";
			break;
		case 555:
			strRank += "Mythic * 419";
			break;
		case 556:
			strRank += "Mythic * 420";
			break;
		case 557:
			strRank += "Mythic * 421";
			break;
		case 558:
			strRank += "Mythic * 422";
			break;
		case 559:
			strRank += "Mythic * 423";
			break;
		case 560:
			strRank += "Mythic * 424";
			break; 
		case 561:
			strRank += "Mythic * 425";
			break;
		case 562:
			strRank += "Mythic * 426";
			break;
		case 563:
			strRank += "Mythic * 427";
			break;
		case 564:
			strRank += "Mythic * 428";
			break;
		case 565:
			strRank += "Mythic * 429";
			break;
		case 566:
			strRank += "Mythic * 430";
			break;
		case 567:
			strRank += "Mythic * 431";
			break;
		case 568:
			strRank += "Mythic * 432";
			break;
		case 569:
			strRank += "Mythic * 433";
			break;
		case 570:
			strRank += "Mythic * 434";
			break;
		case 571:
			strRank += "Mythic * 435";
			break;
		case 572:
			strRank += "Mythic * 436";
			break;
		case 573:
			strRank += "Mythic * 437";
			break;
		case 574:
			strRank += "Mythic * 438";
			break;
		case 575:
			strRank += "Mythic * 439";
			break;
		case 576:
			strRank += "Mythic * 440";
			break;
		case 577:
			strRank += "Mythic * 441";
			break;
		case 578:
			strRank += "Mythic * 442";
			break;
		case 579:
			strRank += "Mythic * 443";
			break;
		case 580:
			strRank += "Mythic * 444";
			break;
		case 581:
			strRank += "Mythic * 445";
			break;
		case 582:
			strRank += "Mythic * 446";
			break;
		case 583:
			strRank += "Mythic * 447";
			break;
		case 584:
			strRank += "Mythic * 448";
			break;
		case 585:
			strRank += "Mythic * 449";
			break;
		case 586:
			strRank += "Mythic * 450";
			break;
		case 587:
			strRank += "Mythic * 451";
			break;
		case 588:
			strRank += "Mythic * 452";
			break;
		case 589:
			strRank += "Mythic * 453";
			break;
		case 590:
			strRank += "Mythic * 454";
			break;
		case 591:
			strRank += "Mythic * 455";
			break;
		case 592:
			strRank += "Mythic * 456";
			break;
		case 593:
			strRank += "Mythic * 457";
			break;
		case 594:
			strRank += "Mythic * 458";
			break;
		case 595:
			strRank += "Mythic * 459";
			break;
		case 596:
			strRank += "Mythic * 460";
			break;
		case 597:
			strRank += "Mythic * 461";
			break;
		case 598:
			strRank += "Mythic * 462";
			break;
		case 599:
			strRank += "Mythic * 463";
			break;
		case 600:
			strRank += "Mythic * 464";
			break;
		case 601:
			strRank += "Mythic * 465";
			break;
		case 602:
			strRank += "Mythic * 466";
			break;
		case 603:
			strRank += "Mythic * 467";
			break;
		case 604:
			strRank += "Mythic * 468";
			break;
		case 605:
			strRank += "Mythic * 469";
			break;
		case 606:
			strRank += "Mythic * 470";
			break;
		case 607:
			strRank += "Mythic * 471";
			break;
		case 608:
			strRank += "Mythic * 472";
			break;
		case 609:
			strRank += "Mythic * 473";
			break;
		case 610:
			strRank += "Mythic * 474";
			break;
		case 611:
			strRank += "Mythic * 475";
			break;
		case 612:
			strRank += "Mythic * 476";
			break;
		case 613:
			strRank += "Mythic * 477";
			break;
		case 614:
			strRank += "Mythic * 478";
			break;
		case 615:
			strRank += "Mythic * 479";
			break;
		case 616:
			strRank += "Mythic * 480";
			break;
		case 617:
			strRank += "Mythic * 481";
			break;
		case 618:
			strRank += "Mythic * 482";
			break;
		case 619:
			strRank += "Mythic * 483";
			break;
		case 620:
			strRank += "Mythic * 484";
			break;
		case 621:
			strRank += "Mythic * 485";
			break;
		case 622:
			strRank += "Mythic * 486";
			break;
		case 623:
			strRank += "Mythic * 487";
			break;
		case 624:
			strRank += "Mythic * 488";
			break;
		case 625:
			strRank += "Mythic * 489";
			break;
		case 626:
			strRank += "Mythic * 490";
			break;
		case 627:
			strRank += "Mythic * 491";
			break;
		case 628:
			strRank += "Mythic * 492";
			break;
		case 629:
			strRank += "Mythic * 493";
			break;
		case 630:
			strRank += "Mythic * 494";
			break;
		case 631:
			strRank += "Mythic * 495";
			break;
		case 632:
			strRank += "Mythic * 496";
			break;
		case 633:
			strRank += "Mythic * 497";
			break;
		case 634:
			strRank += "Mythic * 498";
			break;
		case 635:
			strRank += "Mythic * 499";
			break;
		case 636:
			strRank += "Mythic * 500";
			break;
		default:
			strRank += to_string(uiRankLevel);
	}
	return strRank;
}


std::string MonsterToType(int m_iType) {
	std::string strMonsterType;
	switch(m_iType) {
		case 1:
			strMonsterType += "Minion";
			break;
		case 2:
			strMonsterType += "Bos";
			break;
		case 5:
			strMonsterType += "Buff";
			break;
		default:
			strMonsterType += "";
	}
	return strMonsterType;
}


const char *getSummonid(int id){
	switch(id){

		case 20150://execute
			return "Execute";

		case 20020://retri
			return "Retribution";

		case 20030://inspire
			return "Inspire";

		case 20040://sprint
			return "Sprint";

		case 20050://revitalize
			return "Revitalize";

		case 20060://aegis
			return "Aegis";

		case 20070://stun
			return "Petrify";

		case 20080://purify
			return "Purify";

		case 20140://flameshoot
			return "FlameShoot";

		case 20100://flicker
			return "Flicker";

		case 20160://arrival
			return "Arrival";

		case 20190://vengeance
			return "Vengeance";
			break;
		default:
			return " - ";
	}
}



const char *getHeroId(int id){
	switch(id) {
		case 1:
			return  "1";
			break;
		case 2:
			return "2";
			break;
		case 3:
			return  "3";
			break;
		case 4:
			return  "4";
			break;
		case 5:
			return  "5";
			break;
		case 6:
			return  "6";
			break;
		case 7:
			return  "7";
			break;
		case 8:
			return  "8";
			break;
		case 9:
			return  "9";
			break;
		case 10:
			return  "10";
			break;
		default:
			return " Uknown State ";
	}
}
int ListSummonSkillId[] = {
		20150,
		20020,
		20030,
		20040,
		20050,
		20060,
		20070,
		20080,
		20140,
		20100,
		20160,
		20190
};

bool SpellIdExist(int iValue) {
	return std::find(std::begin(ListSummonSkillId), std::end(ListSummonSkillId), iValue) != std::end(ListSummonSkillId);
}
std::string HeroIDToString(int heroid) {
	std::string strHero;
//Grup MpType Synergy

	switch(heroid) {
		case 37:
			strHero += "Group1";
			break;
		case 80:
			strHero += "Group1";
			break;
		case 821:
			strHero += "Group1";
			break;
		case 4037:
			strHero += "Group1";
			break;
		case 5180:
			strHero += "Group1";
			break;
		case 5141:
			strHero += "Group1";
			break;
		case 41:
			strHero += "Group1";
			break;
		case 45:
			strHero += "Group1";
			break;
		case 4045:
			strHero += "Group1";
			break;


		case 85:
			strHero += "Group1";
			break;
		case 455:
			strHero += "Group1";
			break;
		case 458:
			strHero += "Group1";
			break;
		case 463:
			strHero += "Group1";
			break;
		case 472:
			strHero += "Group1";
			break;
		case 493:
			strHero += "Group1";
			break;
		case 510:
			strHero += "Group1";
			break;
		case 709:
			strHero += "Group1";
			break;
		case 826:
			strHero += "Group1";
			break;
		case 837:
			strHero += "Group1";
			break;
		case 846:
			strHero += "Group1";
			break;
		case 811:
			strHero += "Group1";
			break;
		case 4019:
			strHero += "Group1";
			break;
		case 5119:
			strHero += "Group1";
			break;
		case 19:
			strHero += "Group1";
			break;
		default:
			strHero += "";
	}
	return strHero;
}


const char *getRankName(int id){
	switch(id){

		case 1:
			return "Warrior III * 0";

		case 2:
			return "Warrior III * 1";
		case 3:
			return "Warrior III * 2";
		case 4:
			return "Warrior III * 3";
		case 5:
			return "Warrior II * 1";
		case 6:
			return "Warrior II * 2";
		case 7:
			return "Warrior II * 3";
		case 8:
			return "Warrior I * 1";
		case 9:
			return "Warrior I * 2";
		case 10:
			return "Warrior I * 3";
		case 11:
			return "Elite III * 0";
		case 12:
			return "Elite III * 1";
		case 13:
			return "Elite III * 2";
		case 14:
			return "Elite III * 3";
		case 15:
			return "Elite III * 4";
		case 16:
			return "Elite II * 0";
		case 17:
			return "Elite II * 1";
		case 18:
			return "Elite II * 2";
		case 19:
			return "Elite II * 3";
		case 20:
			return "Elite II * 4";
		case 21:
			return "Elite I * 0";
		case 22:
			return "Elite I * 1";
		case 23:
			return "Elite I * 2";
		case 24:
			return "Elite I * 3";
		case 25:
			return "Elite I * 4";
		case 26:
			return "Master IV * 0";
		case 27:
			return "Master IV * 1";
		case 28:
			return "Master IV * 2";
		case 29:
			return "Master IV * 3";
		case 30:
			return "Master IV * 4";
		case 31:
			return "Master III * 0";
		case 32:
			return "Master III * 1";
		case 33:
			return "Master III * 2";
		case 34:
			return "Master III * 3";
		case 35:
			return "Master III * 4";
		case 36:
			return "Master II * 0";
		case 37:
			return "Master II * 1";
		case 38:
			return "Master II * 2";
		case 39:
			return "Master II * 3";
		case 40:
			return "Master II * 4";
		case 41:
			return "Master I * 0";
		case 42:
			return "Master I * 1";
		case 43:
			return "Master I * 2";
		case 44:
			return "Master I * 3";
		case 45:
			return "Master I * 4";
		case 46:
			return "Grandmaster V * 0";
		case 47:
			return "Grandmaster V * 1";
		case 48:
			return "Grandmaster V * 2";
		case 49:
			return "Grandmaster V * 3";
		case 50:
			return "Grandmaster V * 4";
		case 51:
			return "Grandmaster V * 5";
		case 52:
			return "Grandmaster IV * 0";
		case 53:
			return "Grandmaster IV * 1";
		case 54:
			return "Grandmaster IV * 2";
		case 55:
			return "Grandmaster IV * 3";
		case 56:
			return "Grandmaster IV * 4";
		case 57:
			return "Grandmaster IV * 5";
		case 58:
			return "Grandmaster III * 0";
		case 59:
			return "Grandmaster III * 1";
		case 60:
			return "Grandmaster III * 2";
		case 61:
			return "Grandmaster III * 3";
		case 62:
			return "Grandmaster III * 4";
		case 63:
			return "Grandmaster III * 5";
		case 64:
			return "Grandmaster II * 0";
		case 65:
			return "Grandmaster II * 1";
		case 66:
			return "Grandmaster II * 2";
		case 67:
			return "Grandmaster II * 3";
		case 68:
			return "Grandmaster II * 4";
		case 69:
			return "Grandmaster II * 5";
		case 70:
			return "Grandmaster I * 0";
		case 71:
			return "Grandmaster I * 1";
		case 72:
			return "Grandmaster I * 2";
		case 73:
			return "Grandmaster I * 3";
		case 74:
			return "Grandmaster I * 4";
		case 75:
			return "Grandmaster I * 5";
		case 76:
			return "Epic V * 0";
		case 77:
			return "Epic V * 1";
		case 78:
			return "Epic V * 2";
		case 79:
			return "Epic V * 3";
		case 80:
			return "Epic V * 4";
		case 81:
			return "Epic V * 5";
		case 82:
			return "Epic IV * 0";
		case 83:
			return "Epic IV * 1";
		case 84:
			return "Epic IV * 2";
		case 85:
			return "Epic IV * 3";
		case 86:
			return "Epic IV * 4";
		case 87:
			return "Epic IV * 5";
		case 88:
			return "Epic III * 0";
		case 89:
			return "Epic III * 1";
		case 90:
			return "Epic III * 2";
		case 91:
			return "Epic III * 3";
		case 92:
			return "Epic III * 4";
		case 93:
			return "Epic III * 5";
		case 94:
			return "Epic II * 0";
		case 95:
			return "Epic II * 1";
		case 96:
			return "Epic II * 2";
		case 97:
			return "Epic II * 3";
		case 98:
			return "Epic II * 4";
		case 99:
			return "Epic II * 5";
		case 100:
			return "Epic I * 0";
		case 101:
			return "Epic I * 1";
		case 102:
			return "Epic I * 2";
		case 103:
			return "Epic I * 3";
		case 104:
			return "Epic I * 4";
		case 105:
			return "Epic I * 5";
		case 106:
			return "Legend V * 0";
		case 107:
			return "Legend V * 1";
		case 108:
			return "Legend V * 2";
		case 109:
			return "Legend V * 3";
		case 110:
			return "Legend V * 4";
		case 111:
			return "Legend V * 5";
		case 112:
			return "Legend IV * 0";
		case 113:
			return "Legend IV * 1";
		case 114:
			return "Legend IV * 2";
		case 115:
			return "Legend IV * 3";
		case 116:
			return "Legend IV * 4";
		case 117:
			return "Legend IV * 5";
		case 118:
			return "Legend III * 0";
		case 119:
			return "Legend III * 1";
		case 120:
			return "Legend III * 2";
		case 121:
			return "Legend III * 3";
		case 122:
			return "Legend III * 4";
		case 123:
			return "Legend III * 5";
		case 124:
			return "Legend II * 0";
		case 125:
			return "Legend II * 1";
		case 126:
			return "Legend II * 2";
		case 127:
			return "Legend II * 3";
		case 128:
			return "Legend II * 4";
		case 129:
			return "Legend II * 5";
		case 130:
			return "Legend I * 0";
		case 131:
			return "Legend I * 1";
		case 132:
			return "Legend I * 2";
		case 133:
			return "Legend I * 3";
		case 134:
			return "Legend I * 4";
		case 135:
			return "Legend I * 5";
		case 136:
			return "Mythic * 0";
		case 137:
			return "Mythic * 1";
		case 138:
			return "Mythic * 2";
		case 139:
			return "Mythic * 3";
		case 140:
			return "Mythic * 4";
		case 141:
			return "Mythic * 5";
		case 142:
			return "Mythic * 6";
		case 143:
			return "Mythic * 7";
		case 144:
			return "Mythic * 8";
		case 145:
			return "Mythic * 9";
		case 146:
			return "Mythic * 10";
		case 147:
			return "Mythic * 11";
		case 148:
			return "Mythic * 12";
		case 149:
			return "Mythic * 13";
		case 150:
			return "Mythic * 14";
		case 151:
			return "Mythic * 15";
		case 152:
			return "Mythic * 16";
		case 153:
			return "Mythic * 17";
		case 154:
			return "Mythic * 18";
		case 155:
			return "Mythic * 19";
		case 156:
			return "Mythic * 20";
		case 157:
			return "Mythic * 21";
		case 158:
			return "Mythic * 22";
		case 159:
			return "Mythic * 23";
		case 160:
			return "Mythic * 24";
		case 161:
			return "Mythic * 25";
		case 162:
			return "Mythic * 26";
		case 163:
			return "Mythic * 27";
		case 164:
			return "Mythic * 28";
		case 165:
			return "Mythic * 29";
		case 166:
			return "Mythic * 30";
		case 167:
			return "Mythic * 31";
		case 168:
			return "Mythic * 32";
		case 169:
			return "Mythic * 33";
		case 170:
			return "Mythic * 34";
		case 171:
			return "Mythic * 35";
		case 172:
			return "Mythic * 36";
		case 173:
			return "Mythic * 37";
		case 174:
			return "Mythic * 38";
		case 175:
			return "Mythic * 39";
		case 176:
			return "Mythic * 40";
		case 177:
			return "Mythic * 41";
		case 178:
			return "Mythic * 42";
		case 179:
			return "Mythic * 43";
		case 180:
			return "Mythic * 44";
		case 181:
			return "Mythic * 45";
		case 182:
			return "Mythic * 46";
		case 183:
			return "Mythic * 47";
		case 184:
			return "Mythic * 48";
		case 185:
			return "Mythic * 49";
		case 186:
			return "Mythic * 50";
		case 187:
			return "Mythic * 51";
		case 188:
			return "Mythic * 52";
		case 189:
			return "Mythic * 53";
		case 190:
			return "Mythic * 54";
		case 191:
			return "Mythic * 55";
		case 192:
			return "Mythic * 56";
		case 193:
			return "Mythic * 57";
		case 194:
			return "Mythic * 58";
		case 195:
			return "Mythic * 59";
		case 196:
			return "Mythic * 60";
		case 197:
			return "Mythic * 61";
		case 198:
			return "Mythic * 62";
		case 199:
			return "Mythic * 63";
		case 200:
			return "Mythic * 64";
		case 201:
			return "Mythic * 65";
		case 202:
			return "Mythic * 66";
		case 203:
			return "Mythic * 67";
		case 204:
			return "Mythic * 68";
		case 205:
			return "Mythic * 69";
		case 206:
			return "Mythic * 70";
		case 207:
			return "Mythic * 71";
		case 208:
			return "Mythic * 72";
		case 209:
			return "Mythic * 73";
		case 210:
			return "Mythic * 74";
		case 211:
			return "Mythic * 75";
		case 212:
			return "Mythic * 76";
		case 213:
			return "Mythic * 77";
		case 214:
			return "Mythic * 78";
		case 215:
			return "Mythic * 79";
		case 216:
			return "Mythic * 80";
		case 217:
			return "Mythic * 81";
		case 218:
			return "Mythic * 82";
		case 219:
			return "Mythic * 83";
		case 220:
			return "Mythic * 84";
		case 221:
			return "Mythic * 85";
		case 222:
			return "Mythic * 86";
		case 223:
			return "Mythic * 87";
		case 224:
			return "Mythic * 88";
		case 225:
			return "Mythic * 89";
		case 226:
			return "Mythic * 90";
		case 227:
			return "Mythic * 91";
		case 228:
			return "Mythic * 92";
		case 229:
			return "Mythic * 93";
		case 230:
			return "Mythic * 94";
		case 231:
			return "Mythic * 95";
		case 232:
			return "Mythic * 96";
		case 233:
			return "Mythic * 97";
		case 234:
			return "Mythic * 98";
		case 235:
			return "Mythic * 99";
		case 236:
			return "Mythic * 100";
		case 237:
			return "Mythic * 101";
		case 238:
			return "Mythic * 102";
		case 239:
			return "Mythic * 103";
		case 240:
			return "Mythic * 104";
		case 241:
			return "Mythic * 105";
		case 242:
			return "Mythic * 106";
		case 243:
			return "Mythic * 107";
		case 244:
			return "Mythic * 108";
		case 245:
			return "Mythic * 109";
		case 246:
			return "Mythic * 110";
		case 247:
			return "Mythic * 111";
		case 248:
			return "Mythic * 112";
		case 249:
			return "Mythic * 113";
		case 250:
			return "Mythic * 114";
		case 251:
			return "Mythic * 115";
		case 252:
			return "Mythic * 116";
		case 253:
			return "Mythic * 117";
		case 254:
			return "Mythic * 118";
		case 255:
			return "Mythic * 119";
		case 256:
			return "Mythic * 120";
		case 257:
			return "Mythic * 121";
		case 258:
			return "Mythic * 122";
		case 259:
			return "Mythic * 123";
		case 260:
			return "Mythic * 124";
		case 261:
			return "Mythic * 125";
		case 262:
			return "Mythic * 126";
		case 263:
			return "Mythic * 127";
		case 264:
			return "Mythic * 128";
		case 265:
			return "Mythic * 129";
		case 266:
			return "Mythic * 130";
		case 267:
			return "Mythic * 131";
		case 268:
			return "Mythic * 132";
		case 269:
			return "Mythic * 133";
		case 270:
			return "Mythic * 134";
		case 271:
			return "Mythic * 135";
		case 272:
			return "Mythic * 136";
		case 273:
			return "Mythic * 137";
		case 274:
			return "Mythic * 138";
		case 275:
			return "Mythic * 139";
		case 276:
			return "Mythic * 140";
		case 277:
			return "Mythic * 141";
		case 278:
			return "Mythic * 142";
		case 279:
			return "Mythic * 143";
		case 280:
			return "Mythic * 144";
		case 281:
			return "Mythic * 145";
		case 282:
			return "Mythic * 146";
		case 283:
			return "Mythic * 147";
		case 284:
			return "Mythic * 148";
		case 285:
			return "Mythic * 149";
		case 286:
			return "Mythic * 150";
		case 287:
			return "Mythic * 151";
		case 288:
			return "Mythic * 152";
		case 289:
			return "Mythic * 153";
		case 290:
			return "Mythic * 154";
		case 291:
			return "Mythic * 155";
		case 292:
			return "Mythic * 156";
		case 293:
			return "Mythic * 157";
		case 294:
			return "Mythic * 158";
		case 295:
			return "Mythic * 159";
		case 296:
			return "Mythic * 160";
		case 297:
			return "Mythic * 161";
		case 298:
			return "Mythic * 162";
		case 299:
			return "Mythic * 163";
		case 300:
			return "Mythic * 164";
		case 301:
			return "Mythic * 165";
		case 302:
			return "Mythic * 166";
		case 303:
			return "Mythic * 167";
		case 304:
			return "Mythic * 168";
		case 305:
			return "Mythic * 169";
		case 306:
			return "Mythic * 170";
		case 307:
			return "Mythic * 171";
		case 308:
			return "Mythic * 172";
		case 309:
			return "Mythic * 173";
		case 310:
			return "Mythic * 174";
		case 311:
			return "Mythic * 175";
		case 312:
			return "Mythic * 176";
		case 313:
			return "Mythic * 177";
		case 314:
			return "Mythic * 178";
		case 315:
			return "Mythic * 179";
		case 316:
			return "Mythic * 180";
		case 317:
			return "Mythic * 181";
		case 318:
			return "Mythic * 182";
		case 319:
			return "Mythic * 183";
		case 320:
			return "Mythic * 184";
		case 321:
			return "Mythic * 185";
		case 322:
			return "Mythic * 186";
		case 323:
			return "Mythic * 187";
		case 324:
			return "Mythic * 188";
		case 325:
			return "Mythic * 189";
		case 326:
			return "Mythic * 190";
		case 327:
			return "Mythic * 191";
		case 328:
			return "Mythic * 192";
		case 329:
			return "Mythic * 193";
		case 330:
			return "Mythic * 194";
		case 331:
			return "Mythic * 195";
		case 332:
			return "Mythic * 196";
		case 333:
			return "Mythic * 197";
		case 334:
			return "Mythic * 198";
		case 335:
			return "Mythic * 199";
		case 336:
			return "Mythic * 200";
		case 337:
			return "Mythic * 201";
		case 338:
			return "Mythic * 202";
		case 339:
			return "Mythic * 203";
		case 340:
			return "Mythic * 204";
		case 341:
			return "Mythic * 205";
		case 342:
			return "Mythic * 206";
		case 343:
			return "Mythic * 207";
		case 344:
			return "Mythic * 208";
		case 345:
			return "Mythic * 209";
		case 346:
			return "Mythic * 210";
		case 347:
			return "Mythic * 211";
		case 348:
			return "Mythic * 212";
		case 349:
			return "Mythic * 213";
		case 350:
			return "Mythic * 214";
		case 351:
			return "Mythic * 215";
		case 352:
			return "Mythic * 216";
		case 353:
			return "Mythic * 217";
		case 354:
			return "Mythic * 218";
		case 355:
			return "Mythic * 219";
		case 356:
			return "Mythic * 220";
		case 357:
			return "Mythic * 221";
		case 358:
			return "Mythic * 222";
		case 359:
			return "Mythic * 223";
		case 360:
			return "Mythic * 224";
		case 361:
			return "Mythic * 225";
		case 362:
			return "Mythic * 226";
		case 363:
			return "Mythic * 227";
		case 364:
			return "Mythic * 228";
		case 365:
			return "Mythic * 229";
		case 366:
			return "Mythic * 230";
		case 367:
			return "Mythic * 231";
		case 368:
			return "Mythic * 232";
		case 369:
			return "Mythic * 233";
		case 370:
			return "Mythic * 234";
		case 371:
			return "Mythic * 235";
		case 372:
			return "Mythic * 236";
		case 373:
			return "Mythic * 237";
		case 374:
			return "Mythic * 238";
		case 375:
			return "Mythic * 239";
		case 376:
			return "Mythic * 240";
		case 377:
			return "Mythic * 241";
		case 378:
			return "Mythic * 242";
		case 379:
			return "Mythic * 243";
		case 380:
			return "Mythic * 244";
		case 381:
			return "Mythic * 245";
		case 382:
			return "Mythic * 246";
		case 383:
			return "Mythic * 247";
		case 384:
			return "Mythic * 248";
		case 385:
			return "Mythic * 249";
		case 386:
			return "Mythic * 250";
		case 387:
			return "Mythic * 251";
		case 388:
			return "Mythic * 252";
		case 389:
			return "Mythic * 253";
		case 390:
			return "Mythic * 254";
		case 391:
			return "Mythic * 255";
		case 392:
			return "Mythic * 256";
		case 393:
			return "Mythic * 257";
		case 394:
			return "Mythic * 258";
		case 395:
			return "Mythic * 259";
		case 396:
			return "Mythic * 260";
		case 397:
			return "Mythic * 261";
		case 398:
			return "Mythic * 262";
		case 399:
			return "Mythic * 263";
		case 400:
			return "Mythic * 264";
		case 401:
			return "Mythic * 265";
		case 402:
			return "Mythic * 266";
		case 403:
			return "Mythic * 267";
		case 404:
			return "Mythic * 268";
		case 405:
			return "Mythic * 269";
		case 406:
			return "Mythic * 270";
		case 407:
			return "Mythic * 271";
		case 408:
			return "Mythic * 272";
		case 409:
			return "Mythic * 273";
		case 410:
			return "Mythic * 274";
		case 411:
			return "Mythic * 275";
		case 412:
			return "Mythic * 276";
		case 413:
			return "Mythic * 277";
		case 414:
			return "Mythic * 278";
		case 415:
			return "Mythic * 279";
		case 416:
			return "Mythic * 280";
		case 417:
			return "Mythic * 281";
		case 418:
			return "Mythic * 282";
		case 419:
			return "Mythic * 283";
		case 420:
			return "Mythic * 284";
		case 421:
			return "Mythic * 285";
		case 422:
			return "Mythic * 286";
		case 423:
			return "Mythic * 287";
		case 424:
			return "Mythic * 288";
		case 425:
			return "Mythic * 289";
		case 426:
			return "Mythic * 290";
		case 427:
			return "Mythic * 291";
		case 428:
			return "Mythic * 292";
		case 429:
			return "Mythic * 293";
		case 430:
			return "Mythic * 294";
		case 431:
			return "Mythic * 295";
		case 432:
			return "Mythic * 296";
		case 433:
			return "Mythic * 297";
		case 434:
			return "Mythic * 298";
		case 435:
			return "Mythic * 299";
		case 436:
			return "Mythic * 300";
		case 437:
			return "Mythic * 301";
		case 438:
			return "Mythic * 302";
		case 439:
			return "Mythic * 303";
		case 440:
			return "Mythic * 304";
		case 441:
			return "Mythic * 305";
		case 442:
			return "Mythic * 306";
		case 443:
			return "Mythic * 307";
		case 444:
			return "Mythic * 308";
		case 445:
			return "Mythic * 309";
		case 446:
			return "Mythic * 310";
		case 447:
			return "Mythic * 311";
		case 448:
			return "Mythic * 312";
		case 449:
			return "Mythic * 313";
		case 450:
			return "Mythic * 314";
		case 451:
			return "Mythic * 315";
		case 452:
			return "Mythic * 316";
		case 453:
			return "Mythic * 317";
		case 454:
			return "Mythic * 318";
		case 455:
			return "Mythic * 319";
		case 456:
			return "Mythic * 320";
		case 457:
			return "Mythic * 321";
		case 458:
			return "Mythic * 322";
		case 459:
			return "Mythic * 323";
		case 460:
			return "Mythic * 324";
		case 461:
			return "Mythic * 325";
		case 462:
			return "Mythic * 326";
		case 463:
			return "Mythic * 327";
		case 464:
			return "Mythic * 328";
		case 465:
			return "Mythic * 329";
		case 466:
			return "Mythic * 330";
		case 467:
			return "Mythic * 331";
		case 468:
			return "Mythic * 332";
		case 469:
			return "Mythic * 333";
		case 470:
			return "Mythic * 334";
		case 471:
			return "Mythic * 335";
		case 472:
			return "Mythic * 336";
		case 473:
			return "Mythic * 337";
		case 474:
			return "Mythic * 338";
		case 475:
			return "Mythic * 339";
		case 476:
			return "Mythic * 340";
		case 477:
			return "Mythic * 341";
		case 478:
			return "Mythic * 342";
		case 479:
			return "Mythic * 343";
		case 480:
			return "Mythic * 344";
		case 481:
			return "Mythic * 345";
		case 482:
			return "Mythic * 346";
		case 483:
			return "Mythic * 347";
		case 484:
			return "Mythic * 348";
		case 485:
			return "Mythic * 349";
		case 486:
			return "Mythic * 350";
		case 487:
			return "Mythic * 351";
		case 488:
			return "Mythic * 352";
		case 489:
			return "Mythic * 353";
		case 490:
			return "Mythic * 354";
		case 491:
			return "Mythic * 355";
		case 492:
			return "Mythic * 356";
		case 493:
			return "Mythic * 357";
		case 494:
			return "Mythic * 358";
		case 495:
			return "Mythic * 359";
		case 496:
			return "Mythic * 360";
		case 497:
			return "Mythic * 361";
		case 498:
			return "Mythic * 362";
		case 499:
			return "Mythic * 363";
		case 500:
			return "Mythic * 364";
		case 501:
			return "Mythic * 365";
		case 502:
			return "Mythic * 366";
		case 503:
			return "Mythic * 367";
		case 504:
			return "Mythic * 368";
		case 505:
			return "Mythic * 369";
		case 506:
			return "Mythic * 370";
		case 507:
			return "Mythic * 371";
		case 508:
			return "Mythic * 372";
		case 509:
			return "Mythic * 373";
		case 510:
			return "Mythic * 374";
		case 511:
			return "Mythic * 375";
		case 512:
			return "Mythic * 376";
		case 513:
			return "Mythic * 377";
		case 514:
			return "Mythic * 378";
		case 515:
			return "Mythic * 379";
		case 516:
			return "Mythic * 380";
		case 517:
			return "Mythic * 381";
		case 518:
			return "Mythic * 382";
		case 519:
			return "Mythic * 383";
		case 520:
			return "Mythic * 384";
		case 521:
			return "Mythic * 385";
		case 522:
			return "Mythic * 386";
		case 523:
			return "Mythic * 387";
		case 524:
			return "Mythic * 388";
		case 525:
			return "Mythic * 389";
		case 526:
			return "Mythic * 390";
		case 527:
			return "Mythic * 391";
		case 528:
			return "Mythic * 392";
		case 529:
			return "Mythic * 393";
		case 530:
			return "Mythic * 394";
		case 531:
			return "Mythic * 395";
		case 532:
			return "Mythic * 396";
		case 533:
			return "Mythic * 397";
		case 534:
			return "Mythic * 398";
		case 535:
			return "Mythic * 399";
		case 536:
			return "Mythic * 400";
		case 537:
			return "Mythic * 401";
		case 538:
			return "Mythic * 402";
		case 539:
			return "Mythic * 403";
		case 540:
			return "Mythic * 404";
		case 541:
			return "Mythic * 405";
		case 542:
			return "Mythic * 406";
		case 543:
			return "Mythic * 407";
		case 544:
			return "Mythic * 408";
		case 545:
			return "Mythic * 409";
		case 546:
			return "Mythic * 410";
		case 547:
			return "Mythic * 411";
		case 548:
			return "Mythic * 412";
		case 549:
			return "Mythic * 413";
		case 550:
			return "Mythic * 414";
		case 551:
			return "Mythic * 415";
		case 552:
			return "Mythic * 416";
		case 553:
			return "Mythic * 417";
		case 554:
			return "Mythic * 418";
		case 555:
			return "Mythic * 419";
		case 556:
			return "Mythic * 420";
		case 557:
			return "Mythic * 421";
		case 558:
			return "Mythic * 422";
		case 559:
			return "Mythic * 423";
		case 560:
			return "Mythic * 424";
		case 561:
			return "Mythic * 425";
		case 562:
			return "Mythic * 426";
		case 563:
			return "Mythic * 427";
		case 564:
			return "Mythic * 428";
		case 565:
			return "Mythic * 429";
		case 566:
			return "Mythic * 430";
		case 567:
			return "Mythic * 431";
		case 568:
			return "Mythic * 432";
		case 569:
			return "Mythic * 433";
		case 570:
			return "Mythic * 434";
		case 571:
			return "Mythic * 435";
		case 572:
			return "Mythic * 436";
		case 573:
			return "Mythic * 437";
		case 574:
			return "Mythic * 438";
		case 575:
			return "Mythic * 439";
		case 576:
			return "Mythic * 440";
		case 577:
			return "Mythic * 441";
		case 578:
			return "Mythic * 442";
		case 579:
			return "Mythic * 443";
		case 580:
			return "Mythic * 444";
		case 581:
			return "Mythic * 445";
		case 582:
			return "Mythic * 446";
		case 583:
			return "Mythic * 447";
		case 584:
			return "Mythic * 448";
		case 585:
			return "Mythic * 449";
		case 586:
			return "Mythic * 450";
		case 587:
			return "Mythic * 451";
		case 588:
			return "Mythic * 452";
		case 589:
			return "Mythic * 453";
		case 590:
			return "Mythic * 454";
		case 591:
			return "Mythic * 455";
		case 592:
			return "Mythic * 456";
		case 593:
			return "Mythic * 457";
		case 594:
			return "Mythic * 458";
		case 595:
			return "Mythic * 459";
		case 596:
			return "Mythic * 460";
		case 597:
			return "Mythic * 461";
		case 598:
			return "Mythic * 462";
		case 599:
			return "Mythic * 463";
		case 600:
			return "Mythic * 464";
		case 601:
			return "Mythic * 465";
		case 602:
			return "Mythic * 466";
		case 603:
			return "Mythic * 467";
		case 604:
			return "Mythic * 468";
		case 605:
			return "Mythic * 469";
		case 606:
			return "Mythic * 470";
		case 607:
			return "Mythic * 471";
		case 608:
			return "Mythic * 472";
		case 609:
			return "Mythic * 473";
		case 610:
			return "Mythic * 474";
		case 611:
			return "Mythic * 475";
		case 612:
			return "Mythic * 476";
		case 613:
			return "Mythic * 477";
		case 614:
			return "Mythic * 478";
		case 615:
			return "Mythic * 479";
		case 616:
			return "Mythic * 480";
		case 617:
			return "Mythic * 481";
		case 618:
			return "Mythic * 482";
		case 619:
			return "Mythic * 483";
		case 620:
			return "Mythic * 484";
		case 621:
			return "Mythic * 485";
		case 622:
			return "Mythic * 486";
		case 623:
			return "Mythic * 487";
		case 624:
			return "Mythic * 488";
		case 625:
			return "Mythic * 489";
		case 626:
			return "Mythic * 490";
		case 627:
			return "Mythic * 491";
		case 628:
			return "Mythic * 492";
		case 629:
			return "Mythic * 493";
		case 630:
			return "Mythic * 494";
		case 631:
			return "Mythic * 495";
		case 632:
			return "Mythic * 496";
		case 633:
			return "Mythic * 497";
		case 634:
			return "Mythic * 498";
		case 635:
			return "Mythic * 499";
		case 636:
			return "Mythic * 500";
		case 637:
			return "Mythic * 501";
		case 638:
			return "Mythic * 502";
		case 639:
			return "Mythic * 503";
		case 640:
			return "Mythic * 504";
		case 641:
			return "Mythic * 505";
		case 642:
			return "Mythic * 506";
		case 643:
			return "Mythic * 507";
		case 644:
			return "Mythic * 508";
		case 645:
			return "Mythic * 509";
		case 646:
			return "Mythic * 510";
		case 647:
			return "Mythic * 511";
		case 648:
			return "Mythic * 512";
		case 649:
			return "Mythic * 513";
		case 650:
			return "Mythic * 514";
		case 651:
			return "Mythic * 515";
		case 652:
			return "Mythic * 516";
		case 653:
			return "Mythic * 517";
		case 654:
			return "Mythic * 518";
		case 655:
			return "Mythic * 519";
		case 656:
			return "Mythic * 520";
		case 657:
			return "Mythic * 521";
		case 658:
			return "Mythic * 522";
		case 659:
			return "Mythic * 523";
		case 660:
			return "Mythic * 524";
		case 661:
			return "Mythic * 525";
		case 662:
			return "Mythic * 526";
		case 663:
			return "Mythic * 527";
		case 664:
			return "Mythic * 528";
		case 665:
			return "Mythic * 529";
		case 666:
			return "Mythic * 530";
		case 667:
			return "Mythic * 531";
		case 668:
			return "Mythic * 532";
		case 669:
			return "Mythic * 533";
		case 670:
			return "Mythic * 534";
		case 671:
			return "Mythic * 535";
		case 672:
			return "Mythic * 536";
		case 673:
			return "Mythic * 537";
		case 674:
			return "Mythic * 538";
		case 675:
			return "Mythic * 539";
		case 676:
			return "Mythic * 540";
		case 677:
			return "Mythic * 541";
		case 678:
			return "Mythic * 542";
		case 679:
			return "Mythic * 543";
		case 680:
			return "Mythic * 544";
		case 681:
			return "Mythic * 545";
		case 682:
			return "Mythic * 546";
		case 683:
			return "Mythic * 547";
		case 684:
			return "Mythic * 548";
		case 685:
			return "Mythic * 549";
		case 686:
			return "Mythic * 550";
		case 687:
			return "Mythic * 551";
		case 688:
			return "Mythic * 552";
		case 689:
			return "Mythic * 553";
		case 690:
			return "Mythic * 554";
		case 691:
			return "Mythic * 555";
		case 692:
			return "Mythic * 556";
		case 693:
			return "Mythic * 557";
		case 694:
			return "Mythic * 558";
		case 695:
			return "Mythic * 559";
		case 696:
			return "Mythic * 560";
		case 697:
			return "Mythic * 561";
		case 698:
			return "Mythic * 562";
		case 699:
			return "Mythic * 563";
		case 700:
			return "Mythic * 564";
		case 701:
			return "Mythic * 565";
		case 702:
			return "Mythic * 566";
		case 703:
			return "Mythic * 567";
		case 704:
			return "Mythic * 568";
		case 705:
			return "Mythic * 569";
		case 706:
			return "Mythic * 570";
		case 707:
			return "Mythic * 571";
		case 708:
			return "Mythic * 572";
		case 709:
			return "Mythic * 573";
		case 710:
			return "Mythic * 574";
		case 711:
			return "Mythic * 575";
		case 712:
			return "Mythic * 576";
		case 713:
			return "Mythic * 577";
		case 714:
			return "Mythic * 578";
		case 715:
			return "Mythic * 579";
		case 716:
			return "Mythic * 580";
		case 717:
			return "Mythic * 581";
		case 718:
			return "Mythic * 582";
		case 719:
			return "Mythic * 583";
		case 720:
			return "Mythic * 584";
		case 721:
			return "Mythic * 585";
		case 722:
			return "Mythic * 586";
		case 723:
			return "Mythic * 587";
		case 724:
			return "Mythic * 588";
		case 725:
			return "Mythic * 589";
		case 726:
			return "Mythic * 590";
		case 727:
			return "Mythic * 591";
		case 728:
			return "Mythic * 592";
		case 729:
			return "Mythic * 593";
		case 730:
			return "Mythic * 594";
		case 731:
			return "Mythic * 595";
		case 732:
			return "Mythic * 596";
		case 733:
			return "Mythic * 597";
		case 734:
			return "Mythic * 598";
		case 735:
			return "Mythic * 599";
		case 736:
			return "Mythic * 600";
		case 737:
			return "Mythic * 601";
		case 738:
			return "Mythic * 602";
		case 739:
			return "Mythic * 603";
		case 740:
			return "Mythic * 604";
		case 741:
			return "Mythic * 605";
		case 742:
			return "Mythic * 606";
		case 743:
			return "Mythic * 607";
		case 744:
			return "Mythic * 608";
		case 745:
			return "Mythic * 609";
		case 746:
			return "Mythic * 610";
		case 747:
			return "Mythic * 611";
		case 748:
			return "Mythic * 612";
		case 749:
			return "Mythic * 613";
		case 750:
			return "Mythic * 614";
		case 751:
			return "Mythic * 615";
		case 752:
			return "Mythic * 616";
		case 753:
			return "Mythic * 617";
		case 754:
			return "Mythic * 618";
		case 755:
			return "Mythic * 619";
		case 756:
			return "Mythic * 620";
		case 757:
			return "Mythic * 621";
		case 758:
			return "Mythic * 622";
		case 759:
			return "Mythic * 623";
		case 760:
			return "Mythic * 624";
		case 761:
			return "Mythic * 625";
		case 762:
			return "Mythic * 626";
		case 763:
			return "Mythic * 627";
		case 764:
			return "Mythic * 628";
		case 765:
			return "Mythic * 629";
		case 766:
			return "Mythic * 630";
		case 767:
			return "Mythic * 631";
		case 768:
			return "Mythic * 632";
		case 769:
			return "Mythic * 633";
		case 770:
			return "Mythic * 634";
		case 771:
			return "Mythic * 635";
		case 772:
			return "Mythic * 636";
		case 773:
			return "Mythic * 637";
		case 774:
			return "Mythic * 638";
		case 775:
			return "Mythic * 639";
		case 776:
			return "Mythic * 640";
		case 777:
			return "Mythic * 641";
		case 778:
			return "Mythic * 642";
		case 779:
			return "Mythic * 643";
		case 780:
			return "Mythic * 644";
		case 781:
			return "Mythic * 645";
		case 782:
			return "Mythic * 646";
		case 783:
			return "Mythic * 647";
		case 784:
			return "Mythic * 648";
		case 785:
			return "Mythic * 649";
		case 786:
			return "Mythic * 650";
		case 787:
			return "Mythic * 651";
		case 788:
			return "Mythic * 652";
		case 789:
			return "Mythic * 653";
		case 790:
			return "Mythic * 654";
		case 791:
			return "Mythic * 655";
		case 792:
			return "Mythic * 656";
		case 793:
			return "Mythic * 657";
		case 794:
			return "Mythic * 658";
		case 795:
			return "Mythic * 659";
		case 796:
			return "Mythic * 660";
		case 797:
			return "Mythic * 661";
		case 798:
			return "Mythic * 662";
		case 799:
			return "Mythic * 663";
		case 800:
			return "Mythic * 664";
		case 801:
			return "Mythic * 665";
		case 802:
			return "Mythic * 666";
		case 803:
			return "Mythic * 667";
		case 804:
			return "Mythic * 668";
		case 805:
			return "Mythic * 669";
		case 806:
			return "Mythic * 670";
		case 807:
			return "Mythic * 671";
		case 808:
			return "Mythic * 672";
		case 809:
			return "Mythic * 673";
		case 810:
			return "Mythic * 674";
		case 811:
			return "Mythic * 675";
		case 812:
			return "Mythic * 676";
		case 813:
			return "Mythic * 677";
		case 814:
			return "Mythic * 678";
		case 815:
			return "Mythic * 679";
		case 816:
			return "Mythic * 680";
		case 817:
			return "Mythic * 681";
		case 818:
			return "Mythic * 682";
		case 819:
			return "Mythic * 683";
		case 820:
			return "Mythic * 684";
		case 821:
			return "Mythic * 685";
		case 822:
			return "Mythic * 686";
		case 823:
			return "Mythic * 687";
		case 824:
			return "Mythic * 688";
		case 825:
			return "Mythic * 689";
		case 826:
			return "Mythic * 690";
		case 827:
			return "Mythic * 691";
		case 828:
			return "Mythic * 692";
		case 829:
			return "Mythic * 693";
		case 830:
			return "Mythic * 694";
		case 831:
			return "Mythic * 695";
		case 832:
			return "Mythic * 696";
		case 833:
			return "Mythic * 697";
		case 834:
			return "Mythic * 698";
		case 835:
			return "Mythic * 699";
		case 836:
			return "Mythic * 700";
		case 837:
			return "Mythic * 701";
		case 838:
			return "Mythic * 702";
		case 839:
			return "Mythic * 703";
		case 840:
			return "Mythic * 704";
		case 841:
			return "Mythic * 705";
		case 842:
			return "Mythic * 706";
		case 843:
			return "Mythic * 707";
		case 844:
			return "Mythic * 708";
		case 845:
			return "Mythic * 709";
		case 846:
			return "Mythic * 710";
		case 847:
			return "Mythic * 711";
		case 848:
			return "Mythic * 712";
		case 849:
			return "Mythic * 713";
		case 850:
			return "Mythic * 714";
		case 851:
			return "Mythic * 715";
		case 852:
			return "Mythic * 716";
		case 853:
			return "Mythic * 717";
		case 854:
			return "Mythic * 718";
		case 855:
			return "Mythic * 719";
		case 856:
			return "Mythic * 720";
		case 857:
			return "Mythic * 721";
		case 858:
			return "Mythic * 722";
		case 859:
			return "Mythic * 723";
		case 860:
			return "Mythic * 724";
		case 861:
			return "Mythic * 725";
		case 862:
			return "Mythic * 726";
		case 863:
			return "Mythic * 727";
		case 864:
			return "Mythic * 728";
		case 865:
			return "Mythic * 729";
		case 866:
			return "Mythic * 730";
		case 867:
			return "Mythic * 731";
		case 868:
			return "Mythic * 732";
		case 869:
			return "Mythic * 733";
		case 870:
			return "Mythic * 734";
		case 871:
			return "Mythic * 735";
		case 872:
			return "Mythic * 736";
		case 873:
			return "Mythic * 737";
		case 874:
			return "Mythic * 738";
		case 875:
			return "Mythic * 739";
		case 876:
			return "Mythic * 740";
		case 877:
			return "Mythic * 741";
		case 878:
			return "Mythic * 742";
		case 879:
			return "Mythic * 743";
		case 880:
			return "Mythic * 744";
		case 881:
			return "Mythic * 745";
		case 882:
			return "Mythic * 746";
		case 883:
			return "Mythic * 747";
		case 884:
			return "Mythic * 748";
		case 885:
			return "Mythic * 749";
		case 886:
			return "Mythic * 750";
		case 887:
			return "Mythic * 751";
		case 888:
			return "Mythic * 752";
		case 889:
			return "Mythic * 753";
		case 890:
			return "Mythic * 754";
		case 891:
			return "Mythic * 755";
		case 892:
			return "Mythic * 756";
		case 893:
			return "Mythic * 757";
		case 894:
			return "Mythic * 758";
		case 895:
			return "Mythic * 759";
		case 896:
			return "Mythic * 760";
		case 897:
			return "Mythic * 761";
		case 898:
			return "Mythic * 762";
		case 899:
			return "Mythic * 763";
		case 900:
			return "Mythic * 764";
		case 901:
			return "Mythic * 765";
		case 902:
			return "Mythic * 766";
		case 903:
			return "Mythic * 767";
		case 904:
			return "Mythic * 768";
		case 905:
			return "Mythic * 769";
		case 906:
			return "Mythic * 770";
		case 907:
			return "Mythic * 771";
		case 908:
			return "Mythic * 772";
		case 909:
			return "Mythic * 773";
		case 910:
			return "Mythic * 774";
		case 911:
			return "Mythic * 775";
		case 912:
			return "Mythic * 776";
		case 913:
			return "Mythic * 777";
		case 914:
			return "Mythic * 778";
		case 915:
			return "Mythic * 779";
		case 916:
			return "Mythic * 780";
		case 917:
			return "Mythic * 781";
		case 918:
			return "Mythic * 782";
		case 919:
			return "Mythic * 783";
		case 920:
			return "Mythic * 784";
		case 921:
			return "Mythic * 785";
		case 922:
			return "Mythic * 786";
		case 923:
			return "Mythic * 787";
		case 924:
			return "Mythic * 788";
		case 925:
			return "Mythic * 789";
		case 926:
			return "Mythic * 790";
		case 927:
			return "Mythic * 791";
		case 928:
			return "Mythic * 792";
		case 929:
			return "Mythic * 793";
		case 930:
			return "Mythic * 794";
		case 931:
			return "Mythic * 795";
		case 932:
			return "Mythic * 796";
		case 933:
			return "Mythic * 797";
		case 934:
			return "Mythic * 798";
		case 935:
			return "Mythic * 799";
		case 936:
			return "Mythic * 800";
		case 937:
			return "Mythic * 801";
		case 938:
			return "Mythic * 802";
		case 939:
			return "Mythic * 803";
		case 940:
			return "Mythic * 804";
		case 941:
			return "Mythic * 805";
		case 942:
			return "Mythic * 806";
		case 943:
			return "Mythic * 807";
		case 944:
			return "Mythic * 808";
		case 945:
			return "Mythic * 809";
		case 946:
			return "Mythic * 810";
		case 947:
			return "Mythic * 811";
		case 948:
			return "Mythic * 812";
		case 949:
			return "Mythic * 813";
		case 950:
			return "Mythic * 814";
		case 951:
			return "Mythic * 815";
		case 952:
			return "Mythic * 816";
		case 953:
			return "Mythic * 817";
		case 954:
			return "Mythic * 818";
		case 955:
			return "Mythic * 819";
		case 956:
			return "Mythic * 820";
		case 957:
			return "Mythic * 821";
		case 958:
			return "Mythic * 822";
		case 959:
			return "Mythic * 823";
		case 960:
			return "Mythic * 824";
		case 961:
			return "Mythic * 825";
		case 962:
			return "Mythic * 826";
		case 963:
			return "Mythic * 827";
		case 964:
			return "Mythic * 828";
		case 965:
			return "Mythic * 829";
		case 966:
			return "Mythic * 830";
		case 967:
			return "Mythic * 831";
		case 968:
			return "Mythic * 832";
		case 969:
			return "Mythic * 833";
		case 970:
			return "Mythic * 834";
		case 971:
			return "Mythic * 835";
		case 972:
			return "Mythic * 836";
		case 973:
			return "Mythic * 837";
		case 974:
			return "Mythic * 838";
		case 975:
			return "Mythic * 839";
		case 976:
			return "Mythic * 840";
		case 977:
			return "Mythic * 841";
		case 978:
			return "Mythic * 842";
		case 979:
			return "Mythic * 843";
		case 980:
			return "Mythic * 844";
		case 981:
			return "Mythic * 845";
		case 982:
			return "Mythic * 846";
		case 983:
			return "Mythic * 847";
		case 984:
			return "Mythic * 848";
		case 985:
			return "Mythic * 849";
		case 986:
			return "Mythic * 850";
		case 987:
			return "Mythic * 851";
		case 988:
			return "Mythic * 852";
		case 989:
			return "Mythic * 853";
		case 990:
			return "Mythic * 854";
		case 991:
			return "Mythic * 855";
		case 992:
			return "Mythic * 856";
		case 993:
			return "Mythic * 857";
		case 994:
			return "Mythic * 858";
		case 995:
			return "Mythic * 859";
		case 996:
			return "Mythic * 860";
		case 997:
			return "Mythic * 861";
		case 998:
			return "Mythic * 862";
		case 999:
			return "Mythic * 863";
		case 1000:
			return "Mythic * 864";
		case 1001:
			return "Mythic * 865";
		case 1002:
			return "Mythic * 866";
		case 1003:
			return "Mythic * 867";
		case 1004:
			return "Mythic * 868";
		case 1005:
			return "Mythic * 869";
		case 1006:
			return "Mythic * 870";
		case 1007:
			return "Mythic * 871";
		case 1008:
			return "Mythic * 872";
		case 1009:
			return "Mythic * 873";
		case 1010:
			return "Mythic * 874";
		case 1011:
			return "Mythic * 875";
		case 1012:
			return "Mythic * 876";
		case 1013:
			return "Mythic * 877";
		case 1014:
			return "Mythic * 878";
		case 1015:
			return "Mythic * 879";
		case 1016:
			return "Mythic * 880";
		case 1017:
			return "Mythic * 881";
		case 1018:
			return "Mythic * 882";
		case 1019:
			return "Mythic * 883";
		case 1020:
			return "Mythic * 884";
		case 1021:
			return "Mythic * 885";
		case 1022:
			return "Mythic * 886";
		case 1023:
			return "Mythic * 887";
		case 1024:
			return "Mythic * 888";
		case 1025:
			return "Mythic * 889";
		case 1026:
			return "Mythic * 890";
		case 1027:
			return "Mythic * 891";
		case 1028:
			return "Mythic * 892";
		case 1029:
			return "Mythic * 893";
		case 1030:
			return "Mythic * 894";
		case 1031:
			return "Mythic * 895";
		case 1032:
			return "Mythic * 896";
		case 1033:
			return "Mythic * 897";
		case 1034:
			return "Mythic * 898";
		case 1035:
			return "Mythic * 899";
		case 1036:
			return "Mythic * 900";
		case 1037:
			return "Mythic * 901";
		case 1038:
			return "Mythic * 902";
		case 1039:
			return "Mythic * 903";
		case 1040:
			return "Mythic * 904";
		case 1041:
			return "Mythic * 905";
		case 1042:
			return "Mythic * 906";
		case 1043:
			return "Mythic * 907";
		case 1044:
			return "Mythic * 908";
		case 1045:
			return "Mythic * 909";
		case 1046:
			return "Mythic * 910";
		case 1047:
			return "Mythic * 911";
		case 1048:
			return "Mythic * 912";
		case 1049:
			return "Mythic * 913";
		case 1050:
			return "Mythic * 914";
		case 1051:
			return "Mythic * 915";
		case 1052:
			return "Mythic * 916";
		case 1053:
			return "Mythic * 917";
		case 1054:
			return "Mythic * 918";
		case 1055:
			return "Mythic * 919";
		case 1056:
			return "Mythic * 920";
		case 1057:
			return "Mythic * 921";
		case 1058:
			return "Mythic * 922";
		case 1059:
			return "Mythic * 923";
		case 1060:
			return "Mythic * 924";
		case 1061:
			return "Mythic * 925";
		case 1062:
			return "Mythic * 926";
		case 1063:
			return "Mythic * 927";
		case 1064:
			return "Mythic * 928";
		case 1065:
			return "Mythic * 929";
		case 1066:
			return "Mythic * 930";
		case 1067:
			return "Mythic * 931";
		case 1068:
			return "Mythic * 932";
		case 1069:
			return "Mythic * 933";
		case 1070:
			return "Mythic * 934";
		case 1071:
			return "Mythic * 935";
		case 1072:
			return "Mythic * 936";
		case 1073:
			return "Mythic * 937";
		case 1074:
			return "Mythic * 938";
		case 1075:
			return "Mythic * 939";
		case 1076:
			return "Mythic * 940";
		case 1077:
			return "Mythic * 941";
		case 1078:
			return "Mythic * 942";
		case 1079:
			return "Mythic * 943";
		case 1080:
			return "Mythic * 944";
		case 1081:
			return "Mythic * 945";
		case 1082:
			return "Mythic * 946";
		case 1083:
			return "Mythic * 947";
		case 1084:
			return "Mythic * 948";
		case 1085:
			return "Mythic * 949";
		case 1086:
			return "Mythic * 950";
		case 1087:
			return "Mythic * 951";
		case 1088:
			return "Mythic * 952";
		case 1089:
			return "Mythic * 953";
		case 1090:
			return "Mythic * 954";
		case 1091:
			return "Mythic * 955";
		case 1092:
			return "Mythic * 956";
		case 1093:
			return "Mythic * 957";
		case 1094:
			return "Mythic * 958";
		case 1095:
			return "Mythic * 959";
		case 1096:
			return "Mythic * 960";
		case 1097:
			return "Mythic * 961";
		case 1098:
			return "Mythic * 962";
		case 1099:
			return "Mythic * 963";
		case 1100:
			return "Mythic * 964";
		case 1101:
			return "Mythic * 965";
		case 1102:
			return "Mythic * 966";
		case 1103:
			return "Mythic * 967";
		case 1104:
			return "Mythic * 968";
		case 1105:
			return "Mythic * 969";
		case 1106:
			return "Mythic * 970";
		case 1107:
			return "Mythic * 971";
		case 1108:
			return "Mythic * 972";
		case 1109:
			return "Mythic * 973";
		case 1110:
			return "Mythic * 974";
		case 1111:
			return "Mythic * 975";
		case 1112:
			return "Mythic * 976";
		case 1113:
			return "Mythic * 977";
		case 1114:
			return "Mythic * 978";
		case 1115:
			return "Mythic * 979";
		case 1116:
			return "Mythic * 980";
		case 1117:
			return "Mythic * 981";
		case 1118:
			return "Mythic * 982";
		case 1119:
			return "Mythic * 983";
		case 1120:
			return "Mythic * 984";
		case 1121:
			return "Mythic * 985";
		case 1122:
			return "Mythic * 986";
		case 1123:
			return "Mythic * 987";
		case 1124:
			return "Mythic * 988";
		case 1125:
			return "Mythic * 989";
		case 1126:
			return "Mythic * 990";
		case 1127:
			return "Mythic * 991";
		case 1128:
			return "Mythic * 992";
		case 1129:
			return "Mythic * 993";
		case 1130:
			return "Mythic * 994";
		case 1131:
			return "Mythic * 995";
		case 1132:
			return "Mythic * 996";
		case 1133:
			return "Mythic * 997";
		case 1134:
			return "Mythic * 998";
		case 1135:
			return "Mythic * 999";
		default:
			return " - ";
	}
}
const char *getHeroName(int id){
	switch(id){
		case 1:
			return "Miya";
		case 2:
			return "Balmond";
		case 3:
			return "Saber";
		case 4:
			return "Alice";
		case 5:
			return "Nana";
		case 6:
			return "Tigreal";
		case 7:
			return "Alucard";
		case 8:
			return "Karina";
		case 9:
			return "Akai";
		case 10:
			return "Franco";
		case 11:
			return "Bane";
		case 12:
			return "Bruno";
		case 13:
			return "Clint";
		case 14:
			return "Rafaela";
		case 15:
			return "Eudora";
		case 16:
			return "Zilong";
		case 17:
			return "Fanny";
		case 18:
			return "Layla";
		case 19:
			return "Minotaur";
		case 20:
			return "Lolita";
		case 21:
			return "Hayabusa";
		case 22:
			return "Freya";
		case 23:
			return "Gord";
		case 24:
			return "Natalia";
		case 25:
			return "Kagura";
		case 26:
			return "Chou";
		case 27:
			return "Sun";
		case 28:
			return "Alpha";
		case 29:
			return "Ruby";
		case 30:
			return "Yi Sun-shin";
		case 31:
			return "Moskov";
		case 32:
			return "Johnson";
		case 33:
			return "Cyclops";
		case 34:
			return "Estes";
		case 35:
			return "Hilda";
		case 36:
			return "Aurora";
		case 37:
			return "Lapu-Lapu";
		case 38:
			return "Vexana";
		case 39:
			return "Roger";
		case 40:
			return "Karrie";
		case 41:
			return "Gatotkaca";
		case 42:
			return "Harley";
		case 43:
			return "Irithel";
		case 44:
			return "Grock";
		case 45:
			return "Argus";
		case 46:
			return "Odette";
		case 47:
			return "Lancelot";
		case 48:
			return "Diggie";
		case 49:
			return "Hylos";
		case 50:
			return "Zhask";
		case 51:
			return "Helcurt";
		case 52:
			return "Pharsa";
		case 53:
			return "Lesley";
		case 54:
			return "Jawhead";
		case 55:
			return "Angela";
		case 56:
			return "Gusion";
		case 57:
			return "Valir";
		case 58:
			return "Martis";
		case 59:
			return "Uranus";
		case 60:
			return "Hanabi";
		case 61:
			return "Chang'e";
		case 62:
			return "Kaja";
		case 63:
			return "Selena";
		case 64:
			return "Aldous";
		case 65:
			return "Claude";
		case 66:
			return "Vale";
		case 67:
			return "Leomord";
		case 68:
			return "Lunox";
		case 69:
			return "Hanzo";
		case 70:
			return "Belerick";
		case 71:
			return "Kimmy";
		case 72:
			return "Thamuz";
		case 73:
			return "Harith";
		case 74:
			return "Minsitthar";
		case 75:
			return "Kadita";
		case 76:
			return "Faramis";
		case 77:
			return "Badang";
		case 78:
			return "Khufra";
		case 79:
			return "Granger";
		case 80:
			return "Guinevere";
		case 81:
			return "Esmeralda";
		case 82:
			return "Terizla";
		case 83:
			return "X.Borg";
		case 84:
			return "Ling";
		case 85:
			return "Dyrroth";
		case 86:
			return "Lylia";
		case 87:
			return "Baxia";
		case 88:
			return "Masha";
		case 89:
			return "Wanwan";
		case 90:
			return "Silvanna";
		case 91:
			return "Cecilion";
		case 92:
			return "Carmilla";
		case 93:
			return "Atlas";
		case 94:
			return "Popol and Kupa";
		case 95:
			return "Yu Zhong";
		case 96:
			return "Luo Yi";
		case 97:
			return "Benedetta";
		case 98:
			return "Khaleed";
		case 99:
			return "Barats";
		case 100:
			return "Brody";
		case 101:
			return "Yve";
		case 102:
			return "Mathilda";
		case 103:
			return "Paquito";
		case 104:
			return "Gloo";
		case 105:
			return "Beatrix";
		case 106:
			return "Phoveus";
		case 107:
			return "Natan";
		case 108:
			return "Aulus";
		case 109:
			return "Aamon";
		case 110:
			return "Valentina";
		case 111:
			return "Edith";
		case 112:
			return "Floryn";
		case 113:
			return "Yin";
		case 114:
			return "Melissa";
		case 115:
			return "Xavier";
		case 116:
			return "Julian";
		case 117:
			return "Fredrinn";
		case 118:
			return "Joy";
		case 188:
			return "Test Swordsman";
		case 400:
			return "Miya";
		case 401:
			return "Balmond";
		case 402:
			return "Saber";
		case 403:
			return "Alice";
		case 404:
			return "Nana";
		case 405:
			return "Tigreal";
		case 406:
			return "Alucard";
		case 407:
			return "Karina";
		case 408:
			return "Akai";
		case 409:
			return "Franco";
		case 410:
			return "Bane";
		case 411:
			return "Alice";
		case 414:
			return "Rafaela";
		case 416:
			return "Zilong";
		case 418:
			return "Layla";
		case 419:
			return "Layla";
		case 420:
			return "Lolita";
		case 422:
			return "Freya";
		case 423:
			return "Layla";
		case 424:
			return "Hilda";
		case 425:
			return "Alucard";
		case 426:
			return "Balmond";
		case 427:
			return "Alice";
		case 428:
			return "Alice";
		case 429:
			return "Freya";
		case 430:
			return "Layla";
		case 431:
			return "Alice";
		case 432:
			return "Balmond";
		case 433:
			return "Balmond";
		case 434:
			return "Miya";
		case 435:
			return "Aurora";
		case 436:
			return "Alice";
		case 437:
			return "Layla";
		case 438:
			return "Miya";
		case 439:
			return "Miya";
		case 440:
			return "Eudora";
		case 441:
			return "Balmond";
		case 442:
			return "Balmond";
		case 443:
			return "Eudora";
		case 444:
			return "Balmond";
		case 445:
			return "Eudora";
		case 446:
			return "Layla";
		case 447:
			return "Zilong";
		case 448:
			return "Alice";
		case 449:
			return "Balmond";
		case 450:
			return "Balmond";
		case 451:
			return "Alice";
		case 452:
			return "Thamuz";
		case 453:
			return "Tigreal";
		case 454:
			return "Thamuz";
		case 455:
			return "Dyrroth";
		case 456:
			return "Eudora";
		case 457:
			return "Balmond";
		case 458:
			return "Dyrroth";
		case 459:
			return "Alice";
		case 460:
			return "Balmond";
		case 461:
			return "Valir";
		case 462:
			return "Hanabi";
		case 463:
			return "Dyrroth";
		case 464:
			return "Balmond";
		case 465:
			return "Layla";
		case 466:
			return "Layla";
		case 467:
			return "Miya";
		case 468:
			return "Valir";
		case 469:
			return "Hilda";
		case 470:
			return "Hanabi";
		case 471:
			return "Eudora";
		case 472:
			return "Dyrroth";
		case 473:
			return "Balmond";
		case 474:
			return "Alucard";
		case 475:
			return "Alice";
		case 476:
			return "Alucard";
		case 477:
			return "Rafaela";
		case 478:
			return "Tigreal";
		case 479:
			return "Alice";
		case 480:
			return "Balmond";
		case 481:
			return "Hanabi";
		case 482:
			return "Balmond";
		case 483:
			return "Layla";
		case 484:
			return "Tigreal";
		case 485:
			return "Miya";
		case 486:
			return "Hilda";
		case 487:
			return "Zilong";
		case 488:
			return "Eudora";
		case 489:
			return "Rafaela";
		case 490:
			return "Karina";
		case 491:
			return "Layla";
		case 492:
			return "Balmond";
		case 493:
			return "Dyrroth";
		case 494:
			return "Nana";
		case 495:
			return "Alpha";
		case 496:
			return "Layla";
		case 497:
			return "Alpha";
		case 498:
			return "Alice";
		case 499:
			return "Saber";
		case 500:
			return "Eudora";
		case 501:
			return "Balmond";
		case 502:
			return "Tigreal";
		case 503:
			return "Eudora";
		case 504:
			return "Zilong";
		case 505:
			return "Layla";
		case 506:
			return "Saber";
		case 507:
			return "Nana";
		case 508:
			return "Alucard";
		case 509:
			return "Balmond";
		case 510:
			return "Dyrroth";
		case 511:
			return "Balmond";
		case 512:
			return "Balmond";
		case 513:
			return "Balmond";
		case 514:
			return "Eudora";
		case 515:
			return "Miya";
		case 516:
			return "Miya";
		case 517:
			return "Eudora";
		case 518:
			return "Alpha";
		case 601:
			return "Balmond";
		case 701:
			return "Alucard";
		case 702:
			return "Alpha";
		case 703:
			return "Balmond";
		case 704:
			return "Bane";
		case 705:
			return "Layla";
		case 706:
			return "Hilda";
		case 707:
			return "Valir";
		case 708:
			return "Hanabi";
		case 709:
			return "Dyrroth";
		case 710:
			return "Rafaela";
		case 711:
			return "Saber";
		case 712:
			return "Alice";
		case 801:
			return "Balmond";
		case 802:
			return "Saber";
		case 803:
			return "Alice";
		case 804:
			return "Tigreal";
		case 805:
			return "Alucard";
		case 806:
			return "Akai";
		case 807:
			return "Bane";
		case 808:
			return "Bruno";
		case 809:
			return "Zilong";
		case 810:
			return "Fanny";
		case 811:
			return "Minotaur";
		case 812:
			return "Hayabusa";
		case 813:
			return "Freya";
		case 814:
			return "Chou";
		case 815:
			return "Sun";
		case 816:
			return "Alpha";
		case 817:
			return "Ruby";
		case 818:
			return "Johnson";
		case 819:
			return "Cyclops";
		case 820:
			return "Hilda";
		case 821:
			return "Lapu-Lapu";
		case 822:
			return "Miya";
		case 823:
			return "Clint";
		case 824:
			return "Layla";
		case 825:
			return "Karina";
		case 826:
			return "Dyrroth";
		case 827:
			return "Valir";
		case 828:
			return "Hanabi";
		case 829:
			return "Rafaela";
		case 830:
			return "Saber";
		case 831:
			return "Alpha";
		case 832:
			return "Nana";
		case 833:
			return "Tigreal";
		case 834:
			return "Rafaela";
		case 835:
			return "Balmond";
		case 836:
			return "Alice";
		case 837:
			return "Dyrroth";
		case 838:
			return "Alucard";
		case 839:
			return "Miya";
		case 840:
			return "Alpha";
		case 841:
			return "Nana";
		case 842:
			return "Tigreal";
		case 843:
			return "Rafaela";
		case 844:
			return "Balmond";
		case 845:
			return "Alice";
		case 846:
			return "Dyrroth";
		case 847:
			return "Alucard";
		case 848:
			return "Miya";
		case 901:
			return "Spore  ";
		case 902:
			return "Basic Minion";
		case 903:
			return "Ranged Minion";
		case 904:
			return "Ballista Minion";
		case 905:
			return "Giant Mouth Monster";
		case 906:
			return "Crammer";
		case 907:
			return "Goblin";
		case 908:
			return "Crimson Lizard";
		case 909:
			return "Death";
		case 910:
			return "Spinner";
		case 911:
			return "Gargoyle";
		case 912:
			return "Cleaver";
		case 913:
			return "Statued Chaser";
		case 914:
			return "Pioneer";
		case 915:
			return "Statued Shocker";
		case 916:
			return "Core Guard";
		case 1000:
			return "Hanabi";
		case 1001:
			return "Rafaela";
		case 1002:
			return "Miya";
		case 1003:
			return "Saber";
		case 1004:
			return "Balmond";
		case 1005:
			return "Tigreal";
		case 1006:
			return "Miya";
		case 1007:
			return "Balmond";
		case 1008:
			return "Saber";
		case 1009:
			return "Alice";
		case 1010:
			return "Alice";
		case 950:
			return "******";
		case 951:
			return "******";
		case 952:
			return "******";
		case 953:
			return "******";
		case 954:
			return "******";
		case 955:
			return "******";
		case 956:
			return "******";
		case 957:
			return "******";
		case 958:
			return "******";
		case 959:
			return "******";
		case 960:
			return "******";
		case 961:
			return "******";
		case 962:
			return "******";
		case 4001:
			return "Revamped Miya";
		case 4003:
			return "Revamped Saber";
		case 4006:
			return "Revamped Tigreal";
		case 4007:
			return "Revamped Alucard";
		case 4008:
			return "Revamped Karina";
		case 4009:
			return "Revamped Akai";
		case 4011:
			return "Revamped Bane";
		case 4013:
			return "Revamped Clint";
		case 4015:
			return "Revamped Eudora";
		case 4016:
			return "Revamped Zilong";
		case 4018:
			return "Revamped Layla";
		case 4019:
			return "Revamped Minotaur";
		case 4020:
			return "Revamped Lolita";
		case 4021:
			return "Revamped Hayabusa";
		case 4022:
			return "Revamped Freya";
		case 4024:
			return "Revamped Natalia";
		case 4025:
			return "Revamped Kagura";
		case 4028:
			return "Revamped Alpha";
		case 4030:
			return "Revamped Yi Sun-shin";
		case 4037:
			return "Revamped Lapu-Lapu";
		case 4038:
			return "Revamped Vexana";
		case 4045:
			return "Revamped Argus";
		case 4046:
			return "Revamped Odette";
		case 4047:
			return "Revamped Lancelot";
		case 4053:
			return "Revamped Lesley";
		case 4056:
			return "Revamped Gusion";
		case 4067:
			return "Revamped Leomord";
		case 4069:
			return "Revamped Hanzo";
		case 4071:
			return "Revamped Kimmy";
		case 4076:
			return "Revamped Faramis";
		case 4105:
			return "Beatrix - Mode";
		case 4501:
			return "Harper";
		case 4502:
			return "Hunter - Test (discarded)";
		case 4503:
			return "Abe (discarded)";
		case 4504:
			return "Ragnar";
		case 4505:
			return "Remy";
		case 4506:
			return "Eva";
		case 4507:
			return "Abe ";
		case 4508:
			return "Eggie (discarded)";
		case 4509:
			return "Lil' Harley (discarded)";
		case 4510:
			return "Mavis";
		case 4511:
			return "Eggie";
		case 4512:
			return "Buss";
		case 4514:
			return "Benny";
		case 4515:
			return "Pao";
		case 4516:
			return "Yuki";
		case 4517:
			return "Connie";
		case 4518:
			return "Bersi";
		case 4519:
			return "Brown";
		case 4520:
			return "Saki";
		case 4521:
			return "Kaboom";
		case 4522:
			return "Rya";
		case 4523:
			return "Tharz";
		case 4524:
			return "Austus";
		case 4525:
			return "Dubi";
		case 4526:
			return "Asta";
		case 4601:
			return "Mini Ling";
		case 5001:
			return "Harper";
		case 5002:
			return "Harper O'Lantern";
		case 5003:
			return "Chevalier Harper";
		case 5101:
			return "Miya";
		case 5104:
			return "Alice";
		case 5105:
			return "Nana";
		case 5106:
			return "Tigreal";
		case 5110:
			return "Franco";
		case 5114:
			return "Rafaela";
		case 5115:
			return "Eudora";
		case 5116:
			return "Zilong";
		case 5118:
			return "Layla";
		case 5119:
			return "Minotaur";
		case 5121:
			return "Hayabusa";
		case 5122:
			return "Freya";
		case 5124:
			return "Natalia";
		case 5129:
			return "Ruby";
		case 5131:
			return "Moskov";
		case 5132:
			return "Johnson";
		case 5133:
			return "Cyclops";
		case 5134:
			return "Estes";
		case 5136:
			return "Aurora";
		case 5138:
			return "Vexana";
		case 5139:
			return "Roger";
		case 5140:
			return "Karrie";
		case 5141:
			return "Gatotkaca";
		case 5143:
			return "Irithel";
		case 5146:
			return "Odette";
		case 5147:
			return "Lancelot";
		case 5148:
			return "Diggie";
		case 5150:
			return "Zhask";
		case 5151:
			return "Helcurt";
		case 5154:
			return "Jawhead";
		case 5155:
			return "Angela";
		case 5156:
			return "Gusion";
		case 5158:
			return "Martis";
		case 5160:
			return "Hanabi";
		case 5161:
			return "Chang'e";
		case 5162:
			return "Kaja";
		case 5163:
			return "Selena";
		case 5164:
			return "Aldous";
		case 5167:
			return "Leomord";
		case 5170:
			return "Belerick";
		case 5172:
			return "Thamuz";
		case 5173:
			return "Harith";
		case 5175:
			return "Kadita";
		case 5178:
			return "Khufra";
		case 5180:
			return "Guinevere";
		case 5181:
			return "Esmeralda";
		case 5182:
			return "Terizla";
		case 5183:
			return "X.Borg";
		case 5184:
			return "Ling";
		case 5189:
			return "Wanwan";
		case 5200:
			return "Attack Synergy Button";
		case 5201:
			return "No need to translate";
		case 5202:
			return "Switch DPS and Synergy";
		case 5301:
			return "Beatrix";
		case 6301:
			return "Beatrix (Military Blade)";
		case 6302:
			return "Beatrix (Shotgun)";
		case 6303:
			return "Beatrix (Rocket Launcher)";
		case 6304:
			return "Beatrix (Submachine Gun)";
		case 6305:
			return "Beatrix (Sniper Rifle)";
		case 6306:
			return "Beatrix (Boomerang)";
		case 6307:
			return "Beatrix (Energy Gun)";
		case 6308:
			return "Beatrix (Laser Gun)";
		case 6309:
			return "Beatrix (Flamethrower)";
		case 9994:
			return "Photographer Layla";
		case 9995:
			return "Bot Layla";
		case 9996:
			return "Bot Zilong";
		case 9997:
			return "Bot Tigreal";
		case 9998:
			return "Bot Zilong";
		case 9999:
			return "Bot Layla";
		case 2002:
			return "Lord"; // Lord
		case 2003:
			return "Turtle"; // Turtle
		case 2004:
			return "Fiend"; //Red Buff
		case 2005:
			return "Serpent"; //Blue Buff
		case 2006:
			return "Scaled Lizard"; //Green Blubuff
		case 2008:
			return "Crammer"; //
		case 2009:
			return "RockUrsa"; //Green RedBuff
		case 2011:
			return "Crab"; //Creep Gold
		case 2012:
			return "Serpent Kids"; //Small BlueBuff
		case 2013:
			return "Gold Buff"; //Creep Gold
		case 2056:
			return "Litowanderer";
		case 2059:
			return "Small Crammer";
		case 6032:
			return "Buff";
		case 6034:
			return "Buff";
		case 2072:
			return "Lithowanderer";
			break;
		default:
			return " - ";
	}
}
