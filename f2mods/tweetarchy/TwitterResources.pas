unit TwitterResources;

interface
uses Windows, Messages;

type
 TTwitPost = record
    created_at: string;
    timestamp : TDateTime;
    text : string;
    id : string;
    source : string;
    truncated : boolean;
    favorited : boolean;
   end;
  TTwitUser = record
    id : string;
    name : string;
    screen_name: string;
    location: string;
    description: string;
    profile_image_url: string;
    url: string;
    uprotected: boolean;
   end;
  TTweetData = class(TObject)
   Post : TTwitPost;
   user : TTwitUser;
   end;


function FormatTweet(data : TTweetData): string;


const 
   WM_CLSYNCHRONIZE = WM_USER + 56;

resourcestring
 RES_URL_Base = 'http://twitter.com';
 RES_URL_Timeline = '/statuses/friends_timeline.xml';
 RES_URL_Update = '/statuses/update.xml'; //POST - 'status' = new post
 RES_URL_Friends = '';
 RES_URL_DirectMessages = '';
//Statuses
 STAT_CheckingTweets = 'Checking for new tweets...';
 STAT_DoneChecking = 'Check completed.';
 STAT_Posting = 'Posting tweet...';
 STAT_Done = 'Done.';
 STAT_Err = 'Error: ';
 STAT_UpdateLimitHit = 'Update limit hit.';
 STAT_APIGone = 'API not found! (Update Halted)';
 STAT_NoAuth = 'Unable to log in! (Update Halted)';
 STAT_NeedAccount = 'Please set up your account!';
//INI Files
 RES_IniFile = 'Twitter.ini';
//INI Sections
 SEC_Main = 'Main';
//INI Keys
 KEY_PW = 'PWord';
 KEY_User = 'User';
 KEY_RefTimer = 'RefTimer';
 KEY_AutoCheck = 'AutoRefresh';
 KEY_StartCheck = 'InitialCheck';
 KEY_NotifyNewTweet = 'NotifyNew';
 KEY_NTSoundFile = 'NofifyNewSound';
 KEY_ShowBuffer = 'ShowInBuffer';
//Commands
 CMD_Tweet = 'TWEET';
//Messages
 MSG_NeedContent = '[TW] A tweet has to have some content!';
 MSG_PostAddedQueue = '[TW] Post queued and being sent.';
 MSG_NeedLogin = '[TW] Tweets can''t be sent without setting up your login information!';
 MSG_NewTweet = '[TW] ';
//Sounds
 SND_NewTweet = 'newtweet.wav';
//
 RGEX_Linkify = '(@)(\w+)';
 REP_Linkify = '$1<a href="http://www.twitter.com/$2">$2</a>';


const
 MSG_About = '[TW] Tweetarchy, Twitter Module for Furnarchy. Version ';
 RES_Ver = '1.1.0';
 RES_UA = 'Tweetarchy/'+RES_Ver;
 RES_About = MSG_About+RES_Ver;
 //
 DEF_ReadCacheCount = 200;
 DEF_DisplayCount = 60;

var
 URL_Timeline : string;

implementation

function FormatTweet(data : TTweetData): string;
begin
result := data.user.screen_name + #13#10 + data.post.text;
end;

initialization
URL_Timeline := RES_URL_Base + RES_URL_Timeline;
end.
