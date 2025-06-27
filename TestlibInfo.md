# Testlib说明
## 以下是testlib.h可用于评测机的一些说明：

### Exit Code Information

```c++
#ifndef OK_EXIT_CODE
#   ifdef CONTESTER
#       define OK_EXIT_CODE 0xAC
#   else
#       define OK_EXIT_CODE 0
#   endif
#endif

#ifndef WA_EXIT_CODE
#   ifdef EJUDGE
#       define WA_EXIT_CODE 5
#   elif defined(CONTESTER)
#       define WA_EXIT_CODE 0xAB
#   else
#       define WA_EXIT_CODE 1
#   endif
#endif

#ifndef PE_EXIT_CODE
#   ifdef EJUDGE
#       define PE_EXIT_CODE 4
#   elif defined(CONTESTER)
#       define PE_EXIT_CODE 0xAA
#   else
#       define PE_EXIT_CODE 2
#   endif
#endif

#ifndef FAIL_EXIT_CODE
#   ifdef EJUDGE
#       define FAIL_EXIT_CODE 6
#   elif defined(CONTESTER)
#       define FAIL_EXIT_CODE 0xA3
#   else
#       define FAIL_EXIT_CODE 3
#   endif
#endif

#ifndef DIRT_EXIT_CODE
#   ifdef EJUDGE
#       define DIRT_EXIT_CODE 6
#   else
#       define DIRT_EXIT_CODE 4
#   endif
#endif

#ifndef POINTS_EXIT_CODE
#   define POINTS_EXIT_CODE 7
#endif

#ifndef UNEXPECTED_EOF_EXIT_CODE
#   define UNEXPECTED_EOF_EXIT_CODE 8
#endif

#ifndef PC_BASE_EXIT_CODE
#   ifdef TESTSYS
#       define PC_BASE_EXIT_CODE 50
#   else
#       define PC_BASE_EXIT_CODE 0
#   endif
#endif

enum TResult {
    _ok = 0,
    _wa = 1,
    _pe = 2,
    _fail = 3,
    _dirt = 4,
    _points = 5,
    _unexpected_eof = 8,
    _partially = 16
};

#define _pc(exitCode) (TResult(_partially + (exitCode)))

int resultExitCode(TResult r) {
    if (r == _ok)
        return OK_EXIT_CODE;
    if (r == _wa)
        return WA_EXIT_CODE;
    if (r == _pe)
        return PE_EXIT_CODE;
    if (r == _fail)
        return FAIL_EXIT_CODE;
    if (r == _dirt)
        return DIRT_EXIT_CODE;
    if (r == _points)
        return POINTS_EXIT_CODE;
    if (r == _unexpected_eof)
#ifdef ENABLE_UNEXPECTED_EOF
        return UNEXPECTED_EOF_EXIT_CODE;
#else
        return PE_EXIT_CODE;
#endif
    if (r >= _partially)
        return PC_BASE_EXIT_CODE + (r - _partially);
    return FAIL_EXIT_CODE;
}
```
注意`_pc` verdict总分为200
### Stderr输出说明
使用testlib.h编写的程序如需要从``stderr``输出评测结果，则遵从以下格式：
```Verdict Str + log```

其中各评测结果对应的字符串如下：
```c++
//truncated code
case _ok:
    errorName = "ok ";
    quitscrS(LightGreen, errorName);
    break;
case _wa:
    errorName = "wrong answer ";
    quitscrS(LightRed, errorName);
    break;
case _pe:
    errorName = "wrong output format ";
    quitscrS(LightRed, errorName);
    break;
case _fail:
    errorName = "FAIL ";
    quitscrS(LightRed, errorName);
    break;
case _dirt:
    errorName = "wrong output format ";
    quitscrS(LightCyan, errorName);
    result = _pe;
    break;
case _points:
    errorName = "points ";
    quitscrS(LightYellow, errorName);
    break;
case _unexpected_eof:
    errorName = "unexpected eof ";
    quitscrS(LightCyan, errorName);
    break;
default:
    if (result >= _partially) {
        errorName = testlib_format_("partially correct (%d) ", pctype);
        isPartial = true;
        quitscrS(LightYellow, errorName);
```
对于``points``的评测结果，在``log``中以特定格式输出得分：
```c++
__testlib_points = __testlib_preparePoints(points);
std::string stringPoints = removeDoubleTrailingZeroes(testlib_format_("%.10f", __testlib_points));

std::string quitMessage;
if (NULL == message || 0 == strlen(message))
    quitMessage = stringPoints;
else
    quitMessage = stringPoints + " " + message;
```
## 以下是关于testlib.h组件的说明
### Generator
1. 命令行：`<generator.exe> [params]`，参数params为自定义，可在程序内读取，同时也用于生成随机种子
2. 正常结束的返回值为0，否则为FAIL_EXIT_CODE
3. IO信息：
    1. 如果不使用`startTest(int index)`，那么无需从stdin输入，stdout输出为生成的数据，若程序正常结束亦无stderr输出（否则程序在stderr输出报错信息）
    2. 如果使用`startTest(int index)`，那么：无需从stdin输入，stdout无输出，若程序正常结束亦无stderr输出（否则程序在stderr输出报错信息）

### Validator

1. 命令行：`<validator.exe> [--testset <testset>] [--group <group>] [--testOverviewLogFileName <fileName>] [--testMarkupFileName <fileName>] [--testCase <testCase>] [--testCaseFileName <fileName>]`，错误的参数将会导致FAIL
2. 若输入格式正确，返回值为0；若程序出错或输入格式错误，返回值为FAIL_EXIT_CODE
3. IO信息：
    从stdin读取测试数据，stdout无输出，当输入格式正确且程序正常运行时stderr无输出，否则输出报错信息

### Checker

1. 命令行：`<checker.exe> [--testset <testset>] [--group <group>] <input-file> <output-file> <answer-file> [<report-file> [-appes]]`，额外的参数会导致FAIL
2. 返回值为verdict对应上述代码中的返回值
3. stdin/stdout无io；结果从stderr输出（若指定了`report-file`则为写入不含verdict的结果到文件，stderr输出See file to check exit message，若还指定`-appes`则以xml格式输出含verdict的结果）
4. 可以从`inf`/`ouf`/`ans`读取`input-file`/`output-file`/`answer-file`

### Interactor

1. 命令行：`<interactor.exe> <input-file> <output-file> [<answer-file> [<report-file> [-appes]]]`，额外的参数会导致FAIL
2. 返回值为verdict对应上述代码中的返回值
3. 交互器从stdin读取程序输出，从stdout写入到程序输入以实现交互，结果从stderr输出（若指定了`report-file`则为写入不含verdict的结果到文件，stderr输出See file to check exit message，若还指定`-appes`则以xml格式输出含verdict的结果）
4. 可以从`inf`/`ans`读取`input-file`/`answer-file`、从`ouf`读取程序输出，通过`tout`向`output-file`写入日志

### Warning

注意部分分Verdict有两种情况：
1. partially correct (using `quitf(_pc(points),msg)`)
2. points (using `quitp(points,msg)`)

前者不可以用返回值判别verdict（返回值是`PC_BASE_EXIT_CODE`+分数），后者不能从返回值读出分数，因此需要读取stderr以判别结果：
1. `partially correct (<points>) <message>`
2. `points <points> <message>`/`points points_info=<points_info(no space)> <message>`
