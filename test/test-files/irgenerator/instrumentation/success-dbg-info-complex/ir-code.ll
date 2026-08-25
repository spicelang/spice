; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Vector = type { %interface.IIterable, ptr, i64, i64 }
%interface.IIterable = type { ptr }
%struct.VectorIterator = type { %interface.IIterator, ptr, i64 }
%interface.IIterator = type { ptr }
%struct.Pair = type { i64, ptr }

@anon.string.0 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'vi.getSize() == 3' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\0A\00", align 4
@anon.string.6 = private unnamed_addr constant [72 x i8] c"Assertion failed: Condition 'pair.getFirst() == 2' evaluated to false.\0A\00", align 4
@anon.string.7 = private unnamed_addr constant [76 x i8] c"Assertion failed: Condition 'pair.getSecond() == 9876' evaluated to false.\0A\00", align 4
@anon.string.8 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\0A\00", align 4
@anon.string.9 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\0A\00", align 4
@anon.string.10 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\0A\00", align 4
@anon.string.11 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\0A\00", align 4
@anon.string.12 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\0A\00", align 4
@anon.string.13 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\0A\00", align 4
@anon.string.14 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == -99' evaluated to false.\0A\00", align 4
@anon.string.15 = private unnamed_addr constant [65 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\0A\00", align 4
@anon.string.16 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(0) == 123' evaluated to false.\0A\00", align 4
@anon.string.17 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'vi.get(1) == 4321' evaluated to false.\0A\00", align 4
@anon.string.18 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'vi.get(2) == 9876' evaluated to false.\0A\00", align 4
@anon.string.19 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(0) == 124' evaluated to false.\0A\00", align 4
@anon.string.20 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'vi.get(1) == 4322' evaluated to false.\0A\00", align 4
@anon.string.21 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'vi.get(2) == 9877' evaluated to false.\0A\00", align 4
@anon.string.22 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(0) == 124' evaluated to false.\0A\00", align 4
@anon.string.23 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'vi.get(1) == 4323' evaluated to false.\0A\00", align 4
@anon.string.24 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'vi.get(2) == 9879' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4, !dbg !0

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main(i32 %0, ptr %1) #0 !dbg !14 {
  %result = alloca i32, align 4
  %_argc = alloca i32, align 4
  %_argv = alloca ptr, align 8
  %vi = alloca %struct.Vector, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %it = alloca %struct.VectorIterator, align 8
  %pair = alloca %struct.Pair, align 8
  %6 = alloca i32, align 4
  %7 = alloca i32, align 4
  %8 = alloca %struct.VectorIterator, align 8
  %item = alloca i32, align 4
  %9 = alloca %struct.VectorIterator, align 8
  %item1 = alloca ptr, align 8
  %10 = alloca ptr, align 8
  %11 = alloca %struct.VectorIterator, align 8
  %idx = alloca i64, align 8
  %item2 = alloca ptr, align 8
  %pair.addr = alloca %struct.Pair, align 8
  %12 = alloca ptr, align 8
    #dbg_declare(ptr %result, !22, !DIExpression(), !23)
  store i32 0, ptr %result, align 4, !dbg !23
    #dbg_declare(ptr %_argc, !24, !DIExpression(), !23)
  store i32 %0, ptr %_argc, align 4, !dbg !23
    #dbg_declare(ptr %_argv, !25, !DIExpression(), !23)
  store ptr %1, ptr %_argv, align 8, !dbg !23
  call void @_ZN6VectorIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !26
    #dbg_declare(ptr %vi, !27, !DIExpression(), !26)
  store i32 123, ptr %3, align 4, !dbg !37
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %3), !dbg !37
  store i32 4321, ptr %4, align 4, !dbg !38
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %4), !dbg !38
  store i32 9876, ptr %5, align 4, !dbg !39
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %5), !dbg !39
  %13 = call noundef i64 @_ZN6VectorIiE7getSizeEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !40
  %14 = icmp eq i64 %13, 3, !dbg !41
  br i1 %14, label %assert.exit.L12, label %assert.then.L12, !dbg !41, !prof !42

assert.then.L12:                                  ; preds = %2
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !41
  call void @exit(i32 1), !dbg !41
  unreachable, !dbg !41

assert.exit.L12:                                  ; preds = %2
  %16 = call noundef %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !43
  store %struct.VectorIterator %16, ptr %it, align 8, !dbg !43
    #dbg_declare(ptr %it, !44, !DIExpression(), !43)
  %17 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !50
  br i1 %17, label %assert.exit.L16, label %assert.then.L16, !dbg !50, !prof !42

assert.then.L16:                                  ; preds = %assert.exit.L12
  %18 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !50
  call void @exit(i32 1), !dbg !50
  unreachable, !dbg !50

assert.exit.L16:                                  ; preds = %assert.exit.L12
  %19 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !51
  %20 = load i32, ptr %19, align 4, !dbg !52
  %21 = icmp eq i32 %20, 123, !dbg !52
  br i1 %21, label %assert.exit.L17, label %assert.then.L17, !dbg !52, !prof !42

assert.then.L17:                                  ; preds = %assert.exit.L16
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !52
  call void @exit(i32 1), !dbg !52
  unreachable, !dbg !52

assert.exit.L17:                                  ; preds = %assert.exit.L16
  %23 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !53
  %24 = load i32, ptr %23, align 4, !dbg !54
  %25 = icmp eq i32 %24, 123, !dbg !54
  br i1 %25, label %assert.exit.L18, label %assert.then.L18, !dbg !54, !prof !42

assert.then.L18:                                  ; preds = %assert.exit.L17
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !54
  call void @exit(i32 1), !dbg !54
  unreachable, !dbg !54

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !55
  %27 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !56
  %28 = load i32, ptr %27, align 4, !dbg !57
  %29 = icmp eq i32 %28, 4321, !dbg !57
  br i1 %29, label %assert.exit.L20, label %assert.then.L20, !dbg !57, !prof !42

assert.then.L20:                                  ; preds = %assert.exit.L18
  %30 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !57
  call void @exit(i32 1), !dbg !57
  unreachable, !dbg !57

assert.exit.L20:                                  ; preds = %assert.exit.L18
  %31 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !58
  br i1 %31, label %assert.exit.L21, label %assert.then.L21, !dbg !58, !prof !42

assert.then.L21:                                  ; preds = %assert.exit.L20
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !58
  call void @exit(i32 1), !dbg !58
  unreachable, !dbg !58

assert.exit.L21:                                  ; preds = %assert.exit.L20
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !59
  %33 = call noundef %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !60
  store %struct.Pair %33, ptr %pair, align 8, !dbg !60
    #dbg_declare(ptr %pair, !61, !DIExpression(), !60)
  %34 = call noundef ptr @_ZN4PairImRiE8getFirstEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !70
  %35 = load i64, ptr %34, align 8, !dbg !71
  %36 = icmp eq i64 %35, 2, !dbg !71
  br i1 %36, label %assert.exit.L24, label %assert.then.L24, !dbg !71, !prof !42

assert.then.L24:                                  ; preds = %assert.exit.L21
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !71
  call void @exit(i32 1), !dbg !71
  unreachable, !dbg !71

assert.exit.L24:                                  ; preds = %assert.exit.L21
  %38 = call noundef ptr @_ZN4PairImRiE9getSecondEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !72
  %39 = load i32, ptr %38, align 4, !dbg !73
  %40 = icmp eq i32 %39, 9876, !dbg !73
  br i1 %40, label %assert.exit.L25, label %assert.then.L25, !dbg !73, !prof !42

assert.then.L25:                                  ; preds = %assert.exit.L24
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !73
  call void @exit(i32 1), !dbg !73
  unreachable, !dbg !73

assert.exit.L25:                                  ; preds = %assert.exit.L24
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !74
  %42 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !75
  %43 = xor i1 %42, true, !dbg !75
  br i1 %43, label %assert.exit.L27, label %assert.then.L27, !dbg !75, !prof !42

assert.then.L27:                                  ; preds = %assert.exit.L25
  %44 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !75
  call void @exit(i32 1), !dbg !75
  unreachable, !dbg !75

assert.exit.L27:                                  ; preds = %assert.exit.L25
  store i32 321, ptr %6, align 4, !dbg !76
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %6), !dbg !76
  store i32 -99, ptr %7, align 4, !dbg !77
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %7), !dbg !77
  %45 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !78
  br i1 %45, label %assert.exit.L32, label %assert.then.L32, !dbg !78, !prof !42

assert.then.L32:                                  ; preds = %assert.exit.L27
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !78
  call void @exit(i32 1), !dbg !78
  unreachable, !dbg !78

assert.exit.L32:                                  ; preds = %assert.exit.L27
  call void @_Z13op.minusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 3), !dbg !79
  %47 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !80
  %48 = load i32, ptr %47, align 4, !dbg !81
  %49 = icmp eq i32 %48, 123, !dbg !81
  br i1 %49, label %assert.exit.L36, label %assert.then.L36, !dbg !81, !prof !42

assert.then.L36:                                  ; preds = %assert.exit.L32
  %50 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !81
  call void @exit(i32 1), !dbg !81
  unreachable, !dbg !81

assert.exit.L36:                                  ; preds = %assert.exit.L32
  %51 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !82
  br i1 %51, label %assert.exit.L37, label %assert.then.L37, !dbg !82, !prof !42

assert.then.L37:                                  ; preds = %assert.exit.L36
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !82
  call void @exit(i32 1), !dbg !82
  unreachable, !dbg !82

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %53 = load %struct.VectorIterator, ptr %it, align 8, !dbg !83
  call void @_Z16op.plusplus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !83
  %54 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !84
  %55 = load i32, ptr %54, align 4, !dbg !85
  %56 = icmp eq i32 %55, 4321, !dbg !85
  br i1 %56, label %assert.exit.L39, label %assert.then.L39, !dbg !85, !prof !42

assert.then.L39:                                  ; preds = %assert.exit.L37
  %57 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !85
  call void @exit(i32 1), !dbg !85
  unreachable, !dbg !85

assert.exit.L39:                                  ; preds = %assert.exit.L37
  %58 = load %struct.VectorIterator, ptr %it, align 8, !dbg !86
  call void @_Z18op.minusminus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !86
  %59 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !87
  %60 = load i32, ptr %59, align 4, !dbg !88
  %61 = icmp eq i32 %60, 123, !dbg !88
  br i1 %61, label %assert.exit.L41, label %assert.then.L41, !dbg !88, !prof !42

assert.then.L41:                                  ; preds = %assert.exit.L39
  %62 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !88
  call void @exit(i32 1), !dbg !88
  unreachable, !dbg !88

assert.exit.L41:                                  ; preds = %assert.exit.L39
  call void @_Z12op.plusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 4), !dbg !89
  %63 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !90
  %64 = load i32, ptr %63, align 4, !dbg !91
  %65 = icmp eq i32 %64, -99, !dbg !91
  br i1 %65, label %assert.exit.L43, label %assert.then.L43, !dbg !91, !prof !42

assert.then.L43:                                  ; preds = %assert.exit.L41
  %66 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !91
  call void @exit(i32 1), !dbg !91
  unreachable, !dbg !91

assert.exit.L43:                                  ; preds = %assert.exit.L41
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !92
  %67 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !93
  %68 = xor i1 %67, true, !dbg !93
  br i1 %68, label %assert.exit.L45, label %assert.then.L45, !dbg !93, !prof !42

assert.then.L45:                                  ; preds = %assert.exit.L43
  %69 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !93
  call void @exit(i32 1), !dbg !93
  unreachable, !dbg !93

assert.exit.L45:                                  ; preds = %assert.exit.L43
  %70 = call noundef %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !94
  store %struct.VectorIterator %70, ptr %8, align 8, !dbg !94
    #dbg_declare(ptr %item, !96, !DIExpression(), !94)
  br label %foreach.head.L48, !dbg !94

foreach.head.L48:                                 ; preds = %foreach.tail.L48, %assert.exit.L45
  %71 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %8), !dbg !97
  br i1 %71, label %foreach.body.L48, label %foreach.exit.L48, !dbg !97

foreach.body.L48:                                 ; preds = %foreach.head.L48
  %72 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %8), !dbg !97
  %73 = load i32, ptr %72, align 4, !dbg !97
  store i32 %73, ptr %item, align 4, !dbg !97
  %74 = load i32, ptr %item, align 4, !dbg !98
  %75 = add nsw i32 %74, 1, !dbg !98
  store i32 %75, ptr %item, align 4, !dbg !98
  br label %foreach.tail.L48, !dbg !99

foreach.tail.L48:                                 ; preds = %foreach.body.L48
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %8), !dbg !97
  br label %foreach.head.L48, !dbg !97

foreach.exit.L48:                                 ; preds = %foreach.head.L48
  %76 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !100
  %77 = load i32, ptr %76, align 4, !dbg !101
  %78 = icmp eq i32 %77, 123, !dbg !101
  br i1 %78, label %assert.exit.L51, label %assert.then.L51, !dbg !101, !prof !42

assert.then.L51:                                  ; preds = %foreach.exit.L48
  %79 = call i32 (ptr, ...) @printf(ptr @anon.string.16), !dbg !101
  call void @exit(i32 1), !dbg !101
  unreachable, !dbg !101

assert.exit.L51:                                  ; preds = %foreach.exit.L48
  %80 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !102
  %81 = load i32, ptr %80, align 4, !dbg !103
  %82 = icmp eq i32 %81, 4321, !dbg !103
  br i1 %82, label %assert.exit.L52, label %assert.then.L52, !dbg !103, !prof !42

assert.then.L52:                                  ; preds = %assert.exit.L51
  %83 = call i32 (ptr, ...) @printf(ptr @anon.string.17), !dbg !103
  call void @exit(i32 1), !dbg !103
  unreachable, !dbg !103

assert.exit.L52:                                  ; preds = %assert.exit.L51
  %84 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !104
  %85 = load i32, ptr %84, align 4, !dbg !105
  %86 = icmp eq i32 %85, 9876, !dbg !105
  br i1 %86, label %assert.exit.L53, label %assert.then.L53, !dbg !105, !prof !42

assert.then.L53:                                  ; preds = %assert.exit.L52
  %87 = call i32 (ptr, ...) @printf(ptr @anon.string.18), !dbg !105
  call void @exit(i32 1), !dbg !105
  unreachable, !dbg !105

assert.exit.L53:                                  ; preds = %assert.exit.L52
  %88 = call noundef %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !106
  store %struct.VectorIterator %88, ptr %9, align 8, !dbg !106
    #dbg_declare(ptr %item1, !108, !DIExpression(), !106)
  br label %foreach.head.L56, !dbg !106

foreach.head.L56:                                 ; preds = %foreach.tail.L56, %assert.exit.L53
  %89 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %9), !dbg !109
  br i1 %89, label %foreach.body.L56, label %foreach.exit.L56, !dbg !109

foreach.body.L56:                                 ; preds = %foreach.head.L56
  %90 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %9), !dbg !109
    #dbg_declare(ptr %10, !108, !DIExpression(), !109)
  store ptr %90, ptr %10, align 8, !dbg !109
  %91 = load ptr, ptr %10, align 8, !dbg !110
  %92 = load i32, ptr %91, align 4, !dbg !110
  %93 = add nsw i32 %92, 1, !dbg !110
  store i32 %93, ptr %91, align 4, !dbg !110
  br label %foreach.tail.L56, !dbg !111

foreach.tail.L56:                                 ; preds = %foreach.body.L56
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %9), !dbg !109
  br label %foreach.head.L56, !dbg !109

foreach.exit.L56:                                 ; preds = %foreach.head.L56
  %94 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !112
  %95 = load i32, ptr %94, align 4, !dbg !113
  %96 = icmp eq i32 %95, 124, !dbg !113
  br i1 %96, label %assert.exit.L59, label %assert.then.L59, !dbg !113, !prof !42

assert.then.L59:                                  ; preds = %foreach.exit.L56
  %97 = call i32 (ptr, ...) @printf(ptr @anon.string.19), !dbg !113
  call void @exit(i32 1), !dbg !113
  unreachable, !dbg !113

assert.exit.L59:                                  ; preds = %foreach.exit.L56
  %98 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !114
  %99 = load i32, ptr %98, align 4, !dbg !115
  %100 = icmp eq i32 %99, 4322, !dbg !115
  br i1 %100, label %assert.exit.L60, label %assert.then.L60, !dbg !115, !prof !42

assert.then.L60:                                  ; preds = %assert.exit.L59
  %101 = call i32 (ptr, ...) @printf(ptr @anon.string.20), !dbg !115
  call void @exit(i32 1), !dbg !115
  unreachable, !dbg !115

assert.exit.L60:                                  ; preds = %assert.exit.L59
  %102 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !116
  %103 = load i32, ptr %102, align 4, !dbg !117
  %104 = icmp eq i32 %103, 9877, !dbg !117
  br i1 %104, label %assert.exit.L61, label %assert.then.L61, !dbg !117, !prof !42

assert.then.L61:                                  ; preds = %assert.exit.L60
  %105 = call i32 (ptr, ...) @printf(ptr @anon.string.21), !dbg !117
  call void @exit(i32 1), !dbg !117
  unreachable, !dbg !117

assert.exit.L61:                                  ; preds = %assert.exit.L60
  %106 = call noundef %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !118
  store %struct.VectorIterator %106, ptr %11, align 8, !dbg !118
    #dbg_declare(ptr %idx, !120, !DIExpression(), !118)
  store i64 0, ptr %idx, align 8, !dbg !118
    #dbg_declare(ptr %item2, !122, !DIExpression(), !118)
  br label %foreach.head.L63, !dbg !118

foreach.head.L63:                                 ; preds = %foreach.tail.L63, %assert.exit.L61
  %107 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %11), !dbg !123
  br i1 %107, label %foreach.body.L63, label %foreach.exit.L63, !dbg !123

foreach.body.L63:                                 ; preds = %foreach.head.L63
  %pair3 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr %11), !dbg !123
  store %struct.Pair %pair3, ptr %pair.addr, align 8, !dbg !123
  %108 = load i64, ptr %pair.addr, align 8, !dbg !123
  store i64 %108, ptr %idx, align 8, !dbg !123
  %item.addr = getelementptr inbounds nuw %struct.Pair, ptr %pair.addr, i32 0, i32 1, !dbg !123
    #dbg_declare(ptr %12, !122, !DIExpression(), !123)
  %109 = load ptr, ptr %item.addr, align 8, !dbg !123
  store ptr %109, ptr %12, align 8, !dbg !123
  %110 = load i64, ptr %idx, align 8, !dbg !124
  %111 = trunc i64 %110 to i32, !dbg !124
  %112 = load ptr, ptr %12, align 8, !dbg !124
  %113 = load i32, ptr %112, align 4, !dbg !124
  %114 = add nsw i32 %113, %111, !dbg !124
  store i32 %114, ptr %112, align 4, !dbg !124
  br label %foreach.tail.L63, !dbg !125

foreach.tail.L63:                                 ; preds = %foreach.body.L63
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %11), !dbg !123
  br label %foreach.head.L63, !dbg !123

foreach.exit.L63:                                 ; preds = %foreach.head.L63
  %115 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !126
  %116 = load i32, ptr %115, align 4, !dbg !127
  %117 = icmp eq i32 %116, 124, !dbg !127
  br i1 %117, label %assert.exit.L66, label %assert.then.L66, !dbg !127, !prof !42

assert.then.L66:                                  ; preds = %foreach.exit.L63
  %118 = call i32 (ptr, ...) @printf(ptr @anon.string.22), !dbg !127
  call void @exit(i32 1), !dbg !127
  unreachable, !dbg !127

assert.exit.L66:                                  ; preds = %foreach.exit.L63
  %119 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !128
  %120 = load i32, ptr %119, align 4, !dbg !129
  %121 = icmp eq i32 %120, 4323, !dbg !129
  br i1 %121, label %assert.exit.L67, label %assert.then.L67, !dbg !129, !prof !42

assert.then.L67:                                  ; preds = %assert.exit.L66
  %122 = call i32 (ptr, ...) @printf(ptr @anon.string.23), !dbg !129
  call void @exit(i32 1), !dbg !129
  unreachable, !dbg !129

assert.exit.L67:                                  ; preds = %assert.exit.L66
  %123 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !130
  %124 = load i32, ptr %123, align 4, !dbg !131
  %125 = icmp eq i32 %124, 9879, !dbg !131
  br i1 %125, label %assert.exit.L68, label %assert.then.L68, !dbg !131, !prof !42

assert.then.L68:                                  ; preds = %assert.exit.L67
  %126 = call i32 (ptr, ...) @printf(ptr @anon.string.24), !dbg !131
  call void @exit(i32 1), !dbg !131
  unreachable, !dbg !131

assert.exit.L68:                                  ; preds = %assert.exit.L67
  %127 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !132
  call void @_ZN6VectorIiE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !133
  %128 = load i32, ptr %result, align 4, !dbg !133
  ret i32 %128, !dbg !133
}

declare void @_ZN6VectorIiE4ctorEv(ptr)

declare void @_ZN6VectorIiE8pushBackERKi(ptr, ptr)

declare i64 @_ZN6VectorIiE7getSizeEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #2

declare %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr)

declare i1 @_ZN14VectorIteratorIiE7isValidEv(ptr)

declare ptr @_ZN14VectorIteratorIiE3getEv(ptr)

declare void @_ZN14VectorIteratorIiE4nextEv(ptr)

declare %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr)

declare ptr @_ZN4PairImRiE8getFirstEv(ptr)

declare ptr @_ZN4PairImRiE9getSecondEv(ptr)

declare void @_Z13op.minusequalIiiEvR14VectorIteratorIiEi(ptr, i32)

declare void @_Z16op.plusplus.postIiEvR14VectorIteratorIiE(ptr)

declare void @_Z18op.minusminus.postIiEvR14VectorIteratorIiE(ptr)

declare void @_Z12op.plusequalIiiEvR14VectorIteratorIiEi(ptr, i32)

declare ptr @_ZN6VectorIiE3getEj(ptr, i32)

declare void @_ZN6VectorIiE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32))

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!llvm.module.flags = !{!7, !8, !9, !10, !11, !12}
!llvm.ident = !{!13}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !5, line: 70, type: !6, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/instrumentation/success-dbg-info-complex/source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-complex")
!4 = !{!0}
!5 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-complex")
!6 = !DIStringType(name: "printf.str.0", size: 192)
!7 = !{i32 8, !"PIC Level", i32 2}
!8 = !{i32 7, !"PIE Level", i32 2}
!9 = !{i32 7, !"uwtable", i32 2}
!10 = !{i32 7, !"frame-pointer", i32 2}
!11 = !{i32 7, !"Dwarf Version", i32 5}
!12 = !{i32 2, !"Debug Info Version", i32 3}
!13 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!14 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainiPPKc", scope: !5, file: !5, line: 6, type: !15, scopeLine: 6, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !21)
!15 = !DISubroutineType(types: !16)
!16 = !{!17, !17, !18}
!17 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!18 = !DICompositeType(tag: DW_TAG_array_type, baseType: !19, elements: !21)
!19 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 64)
!20 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!21 = !{}
!22 = !DILocalVariable(name: "result", scope: !14, file: !5, line: 6, type: !17)
!23 = !DILocation(line: 6, column: 1, scope: !14)
!24 = !DILocalVariable(name: "_argc", arg: 1, scope: !14, file: !5, line: 6, type: !17)
!25 = !DILocalVariable(name: "_argv", arg: 2, scope: !14, file: !5, line: 6, type: !18)
!26 = !DILocation(line: 8, column: 22, scope: !14)
!27 = !DILocalVariable(name: "vi", scope: !14, file: !5, line: 8, type: !28)
!28 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector<int>", scope: !5, file: !5, line: 26, size: 256, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !29, templateParams: !35, identifier: "struct.Vector")
!29 = !{!30, !32, !34}
!30 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !28, file: !5, line: 27, baseType: !31, size: 64, offset: 64)
!31 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !17, size: 64)
!32 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !28, file: !5, line: 28, baseType: !33, size: 64, offset: 128)
!33 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !28, file: !5, line: 29, baseType: !33, size: 64, offset: 192)
!35 = !{!36}
!36 = !DITemplateTypeParameter(name: "T", type: !17)
!37 = !DILocation(line: 9, column: 17, scope: !14)
!38 = !DILocation(line: 10, column: 17, scope: !14)
!39 = !DILocation(line: 11, column: 17, scope: !14)
!40 = !DILocation(line: 12, column: 12, scope: !14)
!41 = !DILocation(line: 12, column: 28, scope: !14)
!42 = !{!"branch_weights", i32 1048575, i32 1}
!43 = !DILocation(line: 15, column: 14, scope: !14)
!44 = !DILocalVariable(name: "it", scope: !14, file: !5, line: 15, type: !45)
!45 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator<int>", scope: !5, file: !5, line: 423, size: 192, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !46, templateParams: !35, identifier: "struct.VectorIterator")
!46 = !{!47, !49}
!47 = !DIDerivedType(tag: DW_TAG_member, name: "vector", scope: !45, file: !5, line: 424, baseType: !48, size: 64, offset: 64)
!48 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !28, size: 64)
!49 = !DIDerivedType(tag: DW_TAG_member, name: "cursor", scope: !45, file: !5, line: 425, baseType: !33, size: 64, offset: 128)
!50 = !DILocation(line: 16, column: 12, scope: !14)
!51 = !DILocation(line: 17, column: 12, scope: !14)
!52 = !DILocation(line: 17, column: 24, scope: !14)
!53 = !DILocation(line: 18, column: 12, scope: !14)
!54 = !DILocation(line: 18, column: 24, scope: !14)
!55 = !DILocation(line: 19, column: 5, scope: !14)
!56 = !DILocation(line: 20, column: 12, scope: !14)
!57 = !DILocation(line: 20, column: 24, scope: !14)
!58 = !DILocation(line: 21, column: 12, scope: !14)
!59 = !DILocation(line: 22, column: 5, scope: !14)
!60 = !DILocation(line: 23, column: 16, scope: !14)
!61 = !DILocalVariable(name: "pair", scope: !14, file: !5, line: 23, type: !62)
!62 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair<unsigned long,int&>", scope: !5, file: !5, line: 8, size: 128, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !63, templateParams: !67, identifier: "struct.Pair")
!63 = !{!64, !65}
!64 = !DIDerivedType(tag: DW_TAG_member, name: "first", scope: !62, file: !5, line: 9, baseType: !33, size: 64)
!65 = !DIDerivedType(tag: DW_TAG_member, name: "second", scope: !62, file: !5, line: 10, baseType: !66, size: 64, offset: 64)
!66 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !17, size: 64)
!67 = !{!68, !69}
!68 = !DITemplateTypeParameter(name: "V1", type: !33)
!69 = !DITemplateTypeParameter(name: "V2", type: !66)
!70 = !DILocation(line: 24, column: 12, scope: !14)
!71 = !DILocation(line: 24, column: 31, scope: !14)
!72 = !DILocation(line: 25, column: 12, scope: !14)
!73 = !DILocation(line: 25, column: 32, scope: !14)
!74 = !DILocation(line: 26, column: 5, scope: !14)
!75 = !DILocation(line: 27, column: 13, scope: !14)
!76 = !DILocation(line: 30, column: 17, scope: !14)
!77 = !DILocation(line: 31, column: 17, scope: !14)
!78 = !DILocation(line: 32, column: 12, scope: !14)
!79 = !DILocation(line: 35, column: 5, scope: !14)
!80 = !DILocation(line: 36, column: 12, scope: !14)
!81 = !DILocation(line: 36, column: 24, scope: !14)
!82 = !DILocation(line: 37, column: 12, scope: !14)
!83 = !DILocation(line: 38, column: 5, scope: !14)
!84 = !DILocation(line: 39, column: 12, scope: !14)
!85 = !DILocation(line: 39, column: 24, scope: !14)
!86 = !DILocation(line: 40, column: 5, scope: !14)
!87 = !DILocation(line: 41, column: 12, scope: !14)
!88 = !DILocation(line: 41, column: 24, scope: !14)
!89 = !DILocation(line: 42, column: 5, scope: !14)
!90 = !DILocation(line: 43, column: 12, scope: !14)
!91 = !DILocation(line: 43, column: 24, scope: !14)
!92 = !DILocation(line: 44, column: 5, scope: !14)
!93 = !DILocation(line: 45, column: 13, scope: !14)
!94 = !DILocation(line: 48, column: 24, scope: !95)
!95 = distinct !DILexicalBlock(scope: !14, file: !5, line: 48, column: 5)
!96 = !DILocalVariable(name: "item", scope: !95, file: !5, line: 48, type: !17)
!97 = !DILocation(line: 48, column: 5, scope: !95)
!98 = !DILocation(line: 49, column: 9, scope: !95)
!99 = !DILocation(line: 50, column: 5, scope: !95)
!100 = !DILocation(line: 51, column: 19, scope: !14)
!101 = !DILocation(line: 51, column: 25, scope: !14)
!102 = !DILocation(line: 52, column: 19, scope: !14)
!103 = !DILocation(line: 52, column: 25, scope: !14)
!104 = !DILocation(line: 53, column: 19, scope: !14)
!105 = !DILocation(line: 53, column: 25, scope: !14)
!106 = !DILocation(line: 56, column: 25, scope: !107)
!107 = distinct !DILexicalBlock(scope: !14, file: !5, line: 56, column: 5)
!108 = !DILocalVariable(name: "item", scope: !107, file: !5, line: 56, type: !66)
!109 = !DILocation(line: 56, column: 5, scope: !107)
!110 = !DILocation(line: 57, column: 9, scope: !107)
!111 = !DILocation(line: 58, column: 5, scope: !107)
!112 = !DILocation(line: 59, column: 19, scope: !14)
!113 = !DILocation(line: 59, column: 25, scope: !14)
!114 = !DILocation(line: 60, column: 19, scope: !14)
!115 = !DILocation(line: 60, column: 25, scope: !14)
!116 = !DILocation(line: 61, column: 19, scope: !14)
!117 = !DILocation(line: 61, column: 25, scope: !14)
!118 = !DILocation(line: 63, column: 35, scope: !119)
!119 = distinct !DILexicalBlock(scope: !14, file: !5, line: 63, column: 5)
!120 = !DILocalVariable(name: "idx", scope: !119, file: !5, line: 63, type: !121)
!121 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!122 = !DILocalVariable(name: "item", scope: !119, file: !5, line: 63, type: !66)
!123 = !DILocation(line: 63, column: 5, scope: !119)
!124 = !DILocation(line: 64, column: 9, scope: !119)
!125 = !DILocation(line: 65, column: 5, scope: !119)
!126 = !DILocation(line: 66, column: 19, scope: !14)
!127 = !DILocation(line: 66, column: 25, scope: !14)
!128 = !DILocation(line: 67, column: 19, scope: !14)
!129 = !DILocation(line: 67, column: 25, scope: !14)
!130 = !DILocation(line: 68, column: 19, scope: !14)
!131 = !DILocation(line: 68, column: 25, scope: !14)
!132 = !DILocation(line: 70, column: 5, scope: !14)
!133 = !DILocation(line: 71, column: 1, scope: !14)
