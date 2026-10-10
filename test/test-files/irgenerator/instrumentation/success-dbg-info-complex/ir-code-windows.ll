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
define noundef i32 @main(i32 %0, ptr %1) #0 !dbg !15 {
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
  %12 = alloca %struct.Pair, align 8
  %13 = alloca ptr, align 8
    #dbg_declare(ptr %_argc, !22, !DIExpression(), !52)
  store i32 %0, ptr %_argc, align 4, !dbg !52
    #dbg_declare(ptr %_argv, !23, !DIExpression(), !52)
  store ptr %1, ptr %_argv, align 8, !dbg !52
  call void @_ZN6VectorIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !53
    #dbg_declare(ptr %vi, !24, !DIExpression(), !53)
  store i32 123, ptr %3, align 4, !dbg !54
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %3), !dbg !54
  store i32 4321, ptr %4, align 4, !dbg !55
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %4), !dbg !55
  store i32 9876, ptr %5, align 4, !dbg !56
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %5), !dbg !56
  %14 = call noundef i64 @_ZN6VectorIiE7getSizeEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !57
  %15 = icmp eq i64 %14, 3, !dbg !58
  br i1 %15, label %assert.exit.L12, label %assert.then.L12, !dbg !58, !prof !59

assert.then.L12:                                  ; preds = %2
  %16 = call ptr @__acrt_iob_func(i32 2), !dbg !58
  %17 = call i32 (ptr, ptr, ...) @fprintf(ptr %16, ptr @anon.string.0), !dbg !58
  call void @exit(i32 1), !dbg !58
  unreachable, !dbg !58

assert.exit.L12:                                  ; preds = %2
  call void @_ZN6VectorIiE11getIteratorEv(ptr dead_on_unwind writable sret(%struct.VectorIterator) align 8 %it, ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !60
    #dbg_declare(ptr %it, !32, !DIExpression(), !60)
  %18 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !61
  br i1 %18, label %assert.exit.L16, label %assert.then.L16, !dbg !61, !prof !59

assert.then.L16:                                  ; preds = %assert.exit.L12
  %19 = call ptr @__acrt_iob_func(i32 2), !dbg !61
  %20 = call i32 (ptr, ptr, ...) @fprintf(ptr %19, ptr @anon.string.1), !dbg !61
  call void @exit(i32 1), !dbg !61
  unreachable, !dbg !61

assert.exit.L16:                                  ; preds = %assert.exit.L12
  %21 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !62
  %22 = load i32, ptr %21, align 4, !dbg !63
  %23 = icmp eq i32 %22, 123, !dbg !63
  br i1 %23, label %assert.exit.L17, label %assert.then.L17, !dbg !63, !prof !59

assert.then.L17:                                  ; preds = %assert.exit.L16
  %24 = call ptr @__acrt_iob_func(i32 2), !dbg !63
  %25 = call i32 (ptr, ptr, ...) @fprintf(ptr %24, ptr @anon.string.2), !dbg !63
  call void @exit(i32 1), !dbg !63
  unreachable, !dbg !63

assert.exit.L17:                                  ; preds = %assert.exit.L16
  %26 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !64
  %27 = load i32, ptr %26, align 4, !dbg !65
  %28 = icmp eq i32 %27, 123, !dbg !65
  br i1 %28, label %assert.exit.L18, label %assert.then.L18, !dbg !65, !prof !59

assert.then.L18:                                  ; preds = %assert.exit.L17
  %29 = call ptr @__acrt_iob_func(i32 2), !dbg !65
  %30 = call i32 (ptr, ptr, ...) @fprintf(ptr %29, ptr @anon.string.3), !dbg !65
  call void @exit(i32 1), !dbg !65
  unreachable, !dbg !65

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !66
  %31 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !67
  %32 = load i32, ptr %31, align 4, !dbg !68
  %33 = icmp eq i32 %32, 4321, !dbg !68
  br i1 %33, label %assert.exit.L20, label %assert.then.L20, !dbg !68, !prof !59

assert.then.L20:                                  ; preds = %assert.exit.L18
  %34 = call ptr @__acrt_iob_func(i32 2), !dbg !68
  %35 = call i32 (ptr, ptr, ...) @fprintf(ptr %34, ptr @anon.string.4), !dbg !68
  call void @exit(i32 1), !dbg !68
  unreachable, !dbg !68

assert.exit.L20:                                  ; preds = %assert.exit.L18
  %36 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !69
  br i1 %36, label %assert.exit.L21, label %assert.then.L21, !dbg !69, !prof !59

assert.then.L21:                                  ; preds = %assert.exit.L20
  %37 = call ptr @__acrt_iob_func(i32 2), !dbg !69
  %38 = call i32 (ptr, ptr, ...) @fprintf(ptr %37, ptr @anon.string.5), !dbg !69
  call void @exit(i32 1), !dbg !69
  unreachable, !dbg !69

assert.exit.L21:                                  ; preds = %assert.exit.L20
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !70
  call void @_ZN14VectorIteratorIiE6getIdxEv(ptr dead_on_unwind writable sret(%struct.Pair) align 8 %pair, ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !71
    #dbg_declare(ptr %pair, !38, !DIExpression(), !71)
  %39 = call noundef ptr @_ZN4PairImRiE8getFirstEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !72
  %40 = load i64, ptr %39, align 8, !dbg !73
  %41 = icmp eq i64 %40, 2, !dbg !73
  br i1 %41, label %assert.exit.L24, label %assert.then.L24, !dbg !73, !prof !59

assert.then.L24:                                  ; preds = %assert.exit.L21
  %42 = call ptr @__acrt_iob_func(i32 2), !dbg !73
  %43 = call i32 (ptr, ptr, ...) @fprintf(ptr %42, ptr @anon.string.6), !dbg !73
  call void @exit(i32 1), !dbg !73
  unreachable, !dbg !73

assert.exit.L24:                                  ; preds = %assert.exit.L21
  %44 = call noundef ptr @_ZN4PairImRiE9getSecondEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !74
  %45 = load i32, ptr %44, align 4, !dbg !75
  %46 = icmp eq i32 %45, 9876, !dbg !75
  br i1 %46, label %assert.exit.L25, label %assert.then.L25, !dbg !75, !prof !59

assert.then.L25:                                  ; preds = %assert.exit.L24
  %47 = call ptr @__acrt_iob_func(i32 2), !dbg !75
  %48 = call i32 (ptr, ptr, ...) @fprintf(ptr %47, ptr @anon.string.7), !dbg !75
  call void @exit(i32 1), !dbg !75
  unreachable, !dbg !75

assert.exit.L25:                                  ; preds = %assert.exit.L24
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !76
  %49 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !77
  %50 = xor i1 %49, true, !dbg !77
  br i1 %50, label %assert.exit.L27, label %assert.then.L27, !dbg !77, !prof !59

assert.then.L27:                                  ; preds = %assert.exit.L25
  %51 = call ptr @__acrt_iob_func(i32 2), !dbg !77
  %52 = call i32 (ptr, ptr, ...) @fprintf(ptr %51, ptr @anon.string.8), !dbg !77
  call void @exit(i32 1), !dbg !77
  unreachable, !dbg !77

assert.exit.L27:                                  ; preds = %assert.exit.L25
  store i32 321, ptr %6, align 4, !dbg !78
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %6), !dbg !78
  store i32 -99, ptr %7, align 4, !dbg !79
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %7), !dbg !79
  %53 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !80
  br i1 %53, label %assert.exit.L32, label %assert.then.L32, !dbg !80, !prof !59

assert.then.L32:                                  ; preds = %assert.exit.L27
  %54 = call ptr @__acrt_iob_func(i32 2), !dbg !80
  %55 = call i32 (ptr, ptr, ...) @fprintf(ptr %54, ptr @anon.string.9), !dbg !80
  call void @exit(i32 1), !dbg !80
  unreachable, !dbg !80

assert.exit.L32:                                  ; preds = %assert.exit.L27
  call void @_Z13op.minusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 3), !dbg !81
  %56 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !82
  %57 = load i32, ptr %56, align 4, !dbg !83
  %58 = icmp eq i32 %57, 123, !dbg !83
  br i1 %58, label %assert.exit.L36, label %assert.then.L36, !dbg !83, !prof !59

assert.then.L36:                                  ; preds = %assert.exit.L32
  %59 = call ptr @__acrt_iob_func(i32 2), !dbg !83
  %60 = call i32 (ptr, ptr, ...) @fprintf(ptr %59, ptr @anon.string.10), !dbg !83
  call void @exit(i32 1), !dbg !83
  unreachable, !dbg !83

assert.exit.L36:                                  ; preds = %assert.exit.L32
  %61 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !84
  br i1 %61, label %assert.exit.L37, label %assert.then.L37, !dbg !84, !prof !59

assert.then.L37:                                  ; preds = %assert.exit.L36
  %62 = call ptr @__acrt_iob_func(i32 2), !dbg !84
  %63 = call i32 (ptr, ptr, ...) @fprintf(ptr %62, ptr @anon.string.11), !dbg !84
  call void @exit(i32 1), !dbg !84
  unreachable, !dbg !84

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %64 = load %struct.VectorIterator, ptr %it, align 8, !dbg !85
  call void @_Z16op.plusplus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !85
  %65 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !86
  %66 = load i32, ptr %65, align 4, !dbg !87
  %67 = icmp eq i32 %66, 4321, !dbg !87
  br i1 %67, label %assert.exit.L39, label %assert.then.L39, !dbg !87, !prof !59

assert.then.L39:                                  ; preds = %assert.exit.L37
  %68 = call ptr @__acrt_iob_func(i32 2), !dbg !87
  %69 = call i32 (ptr, ptr, ...) @fprintf(ptr %68, ptr @anon.string.12), !dbg !87
  call void @exit(i32 1), !dbg !87
  unreachable, !dbg !87

assert.exit.L39:                                  ; preds = %assert.exit.L37
  %70 = load %struct.VectorIterator, ptr %it, align 8, !dbg !88
  call void @_Z18op.minusminus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !88
  %71 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !89
  %72 = load i32, ptr %71, align 4, !dbg !90
  %73 = icmp eq i32 %72, 123, !dbg !90
  br i1 %73, label %assert.exit.L41, label %assert.then.L41, !dbg !90, !prof !59

assert.then.L41:                                  ; preds = %assert.exit.L39
  %74 = call ptr @__acrt_iob_func(i32 2), !dbg !90
  %75 = call i32 (ptr, ptr, ...) @fprintf(ptr %74, ptr @anon.string.13), !dbg !90
  call void @exit(i32 1), !dbg !90
  unreachable, !dbg !90

assert.exit.L41:                                  ; preds = %assert.exit.L39
  call void @_Z12op.plusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 4), !dbg !91
  %76 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !92
  %77 = load i32, ptr %76, align 4, !dbg !93
  %78 = icmp eq i32 %77, -99, !dbg !93
  br i1 %78, label %assert.exit.L43, label %assert.then.L43, !dbg !93, !prof !59

assert.then.L43:                                  ; preds = %assert.exit.L41
  %79 = call ptr @__acrt_iob_func(i32 2), !dbg !93
  %80 = call i32 (ptr, ptr, ...) @fprintf(ptr %79, ptr @anon.string.14), !dbg !93
  call void @exit(i32 1), !dbg !93
  unreachable, !dbg !93

assert.exit.L43:                                  ; preds = %assert.exit.L41
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !94
  %81 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !95
  %82 = xor i1 %81, true, !dbg !95
  br i1 %82, label %assert.exit.L45, label %assert.then.L45, !dbg !95, !prof !59

assert.then.L45:                                  ; preds = %assert.exit.L43
  %83 = call ptr @__acrt_iob_func(i32 2), !dbg !95
  %84 = call i32 (ptr, ptr, ...) @fprintf(ptr %83, ptr @anon.string.15), !dbg !95
  call void @exit(i32 1), !dbg !95
  unreachable, !dbg !95

assert.exit.L45:                                  ; preds = %assert.exit.L43
  call void @_ZN6VectorIiE11getIteratorEv(ptr dead_on_unwind writable sret(%struct.VectorIterator) align 8 %8, ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !96
  br label %foreach.head.L48, !dbg !96

foreach.head.L48:                                 ; preds = %foreach.tail.L48, %assert.exit.L45
  %85 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %8), !dbg !97
  br i1 %85, label %foreach.body.L48, label %foreach.exit.L48, !dbg !97

foreach.body.L48:                                 ; preds = %foreach.head.L48
    #dbg_declare(ptr %item, !44, !DIExpression(), !97)
  %86 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %8), !dbg !97
  %87 = load i32, ptr %86, align 4, !dbg !97
  store i32 %87, ptr %item, align 4, !dbg !97
  %88 = load i32, ptr %item, align 4, !dbg !98
  %89 = add nsw i32 %88, 1, !dbg !98
  store i32 %89, ptr %item, align 4, !dbg !98
  br label %foreach.tail.L48, !dbg !99

foreach.tail.L48:                                 ; preds = %foreach.body.L48
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %8), !dbg !97
  br label %foreach.head.L48, !dbg !97

foreach.exit.L48:                                 ; preds = %foreach.head.L48
  %90 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !100
  %91 = load i32, ptr %90, align 4, !dbg !101
  %92 = icmp eq i32 %91, 123, !dbg !101
  br i1 %92, label %assert.exit.L51, label %assert.then.L51, !dbg !101, !prof !59

assert.then.L51:                                  ; preds = %foreach.exit.L48
  %93 = call ptr @__acrt_iob_func(i32 2), !dbg !101
  %94 = call i32 (ptr, ptr, ...) @fprintf(ptr %93, ptr @anon.string.16), !dbg !101
  call void @exit(i32 1), !dbg !101
  unreachable, !dbg !101

assert.exit.L51:                                  ; preds = %foreach.exit.L48
  %95 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !102
  %96 = load i32, ptr %95, align 4, !dbg !103
  %97 = icmp eq i32 %96, 4321, !dbg !103
  br i1 %97, label %assert.exit.L52, label %assert.then.L52, !dbg !103, !prof !59

assert.then.L52:                                  ; preds = %assert.exit.L51
  %98 = call ptr @__acrt_iob_func(i32 2), !dbg !103
  %99 = call i32 (ptr, ptr, ...) @fprintf(ptr %98, ptr @anon.string.17), !dbg !103
  call void @exit(i32 1), !dbg !103
  unreachable, !dbg !103

assert.exit.L52:                                  ; preds = %assert.exit.L51
  %100 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !104
  %101 = load i32, ptr %100, align 4, !dbg !105
  %102 = icmp eq i32 %101, 9876, !dbg !105
  br i1 %102, label %assert.exit.L53, label %assert.then.L53, !dbg !105, !prof !59

assert.then.L53:                                  ; preds = %assert.exit.L52
  %103 = call ptr @__acrt_iob_func(i32 2), !dbg !105
  %104 = call i32 (ptr, ptr, ...) @fprintf(ptr %103, ptr @anon.string.18), !dbg !105
  call void @exit(i32 1), !dbg !105
  unreachable, !dbg !105

assert.exit.L53:                                  ; preds = %assert.exit.L52
  call void @_ZN6VectorIiE11getIteratorEv(ptr dead_on_unwind writable sret(%struct.VectorIterator) align 8 %9, ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !106
  br label %foreach.head.L56, !dbg !106

foreach.head.L56:                                 ; preds = %foreach.tail.L56, %assert.exit.L53
  %105 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %9), !dbg !107
  br i1 %105, label %foreach.body.L56, label %foreach.exit.L56, !dbg !107

foreach.body.L56:                                 ; preds = %foreach.head.L56
    #dbg_declare(ptr %item1, !46, !DIExpression(), !107)
  %106 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %9), !dbg !107
    #dbg_declare(ptr %10, !46, !DIExpression(), !107)
  store ptr %106, ptr %10, align 8, !dbg !107
  %107 = load ptr, ptr %10, align 8, !dbg !108
  %108 = load i32, ptr %107, align 4, !dbg !108
  %109 = add nsw i32 %108, 1, !dbg !108
  store i32 %109, ptr %107, align 4, !dbg !108
  br label %foreach.tail.L56, !dbg !109

foreach.tail.L56:                                 ; preds = %foreach.body.L56
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %9), !dbg !107
  br label %foreach.head.L56, !dbg !107

foreach.exit.L56:                                 ; preds = %foreach.head.L56
  %110 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !110
  %111 = load i32, ptr %110, align 4, !dbg !111
  %112 = icmp eq i32 %111, 124, !dbg !111
  br i1 %112, label %assert.exit.L59, label %assert.then.L59, !dbg !111, !prof !59

assert.then.L59:                                  ; preds = %foreach.exit.L56
  %113 = call ptr @__acrt_iob_func(i32 2), !dbg !111
  %114 = call i32 (ptr, ptr, ...) @fprintf(ptr %113, ptr @anon.string.19), !dbg !111
  call void @exit(i32 1), !dbg !111
  unreachable, !dbg !111

assert.exit.L59:                                  ; preds = %foreach.exit.L56
  %115 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !112
  %116 = load i32, ptr %115, align 4, !dbg !113
  %117 = icmp eq i32 %116, 4322, !dbg !113
  br i1 %117, label %assert.exit.L60, label %assert.then.L60, !dbg !113, !prof !59

assert.then.L60:                                  ; preds = %assert.exit.L59
  %118 = call ptr @__acrt_iob_func(i32 2), !dbg !113
  %119 = call i32 (ptr, ptr, ...) @fprintf(ptr %118, ptr @anon.string.20), !dbg !113
  call void @exit(i32 1), !dbg !113
  unreachable, !dbg !113

assert.exit.L60:                                  ; preds = %assert.exit.L59
  %120 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !114
  %121 = load i32, ptr %120, align 4, !dbg !115
  %122 = icmp eq i32 %121, 9877, !dbg !115
  br i1 %122, label %assert.exit.L61, label %assert.then.L61, !dbg !115, !prof !59

assert.then.L61:                                  ; preds = %assert.exit.L60
  %123 = call ptr @__acrt_iob_func(i32 2), !dbg !115
  %124 = call i32 (ptr, ptr, ...) @fprintf(ptr %123, ptr @anon.string.21), !dbg !115
  call void @exit(i32 1), !dbg !115
  unreachable, !dbg !115

assert.exit.L61:                                  ; preds = %assert.exit.L60
  call void @_ZN6VectorIiE11getIteratorEv(ptr dead_on_unwind writable sret(%struct.VectorIterator) align 8 %11, ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !116
    #dbg_declare(ptr %idx, !48, !DIExpression(), !116)
  store i64 0, ptr %idx, align 8, !dbg !116
  br label %foreach.head.L63, !dbg !116

foreach.head.L63:                                 ; preds = %foreach.tail.L63, %assert.exit.L61
  %125 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %11), !dbg !117
  br i1 %125, label %foreach.body.L63, label %foreach.exit.L63, !dbg !117

foreach.body.L63:                                 ; preds = %foreach.head.L63
    #dbg_declare(ptr %item2, !51, !DIExpression(), !117)
  call void @_ZN14VectorIteratorIiE6getIdxEv(ptr dead_on_unwind writable sret(%struct.Pair) align 8 %12, ptr %11), !dbg !117
  %126 = load i64, ptr %12, align 8, !dbg !117
  store i64 %126, ptr %idx, align 8, !dbg !117
  %item.addr = getelementptr inbounds nuw %struct.Pair, ptr %12, i32 0, i32 1, !dbg !117
    #dbg_declare(ptr %13, !51, !DIExpression(), !117)
  %127 = load ptr, ptr %item.addr, align 8, !dbg !117
  store ptr %127, ptr %13, align 8, !dbg !117
  %128 = load i64, ptr %idx, align 8, !dbg !118
  %129 = trunc i64 %128 to i32, !dbg !118
  %130 = load ptr, ptr %13, align 8, !dbg !118
  %131 = load i32, ptr %130, align 4, !dbg !118
  %132 = add nsw i32 %131, %129, !dbg !118
  store i32 %132, ptr %130, align 4, !dbg !118
  br label %foreach.tail.L63, !dbg !119

foreach.tail.L63:                                 ; preds = %foreach.body.L63
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %11), !dbg !117
  br label %foreach.head.L63, !dbg !117

foreach.exit.L63:                                 ; preds = %foreach.head.L63
  %133 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !120
  %134 = load i32, ptr %133, align 4, !dbg !121
  %135 = icmp eq i32 %134, 124, !dbg !121
  br i1 %135, label %assert.exit.L66, label %assert.then.L66, !dbg !121, !prof !59

assert.then.L66:                                  ; preds = %foreach.exit.L63
  %136 = call ptr @__acrt_iob_func(i32 2), !dbg !121
  %137 = call i32 (ptr, ptr, ...) @fprintf(ptr %136, ptr @anon.string.22), !dbg !121
  call void @exit(i32 1), !dbg !121
  unreachable, !dbg !121

assert.exit.L66:                                  ; preds = %foreach.exit.L63
  %138 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !122
  %139 = load i32, ptr %138, align 4, !dbg !123
  %140 = icmp eq i32 %139, 4323, !dbg !123
  br i1 %140, label %assert.exit.L67, label %assert.then.L67, !dbg !123, !prof !59

assert.then.L67:                                  ; preds = %assert.exit.L66
  %141 = call ptr @__acrt_iob_func(i32 2), !dbg !123
  %142 = call i32 (ptr, ptr, ...) @fprintf(ptr %141, ptr @anon.string.23), !dbg !123
  call void @exit(i32 1), !dbg !123
  unreachable, !dbg !123

assert.exit.L67:                                  ; preds = %assert.exit.L66
  %143 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !124
  %144 = load i32, ptr %143, align 4, !dbg !125
  %145 = icmp eq i32 %144, 9879, !dbg !125
  br i1 %145, label %assert.exit.L68, label %assert.then.L68, !dbg !125, !prof !59

assert.then.L68:                                  ; preds = %assert.exit.L67
  %146 = call ptr @__acrt_iob_func(i32 2), !dbg !125
  %147 = call i32 (ptr, ptr, ...) @fprintf(ptr %146, ptr @anon.string.24), !dbg !125
  call void @exit(i32 1), !dbg !125
  unreachable, !dbg !125

assert.exit.L68:                                  ; preds = %assert.exit.L67
  %148 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !126
  call void @_ZN6VectorIiE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !127
  ret i32 0, !dbg !127
}

declare void @_ZN6VectorIiE4ctorEv(ptr)

declare void @_ZN6VectorIiE8pushBackERKi(ptr, ptr)

declare i64 @_ZN6VectorIiE7getSizeEv(ptr)

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: nounwind
declare noundef ptr @__acrt_iob_func(i32 noundef) #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

declare void @_ZN6VectorIiE11getIteratorEv(ptr dead_on_unwind noalias writable sret(%struct.VectorIterator) align 8, ptr)

declare i1 @_ZN14VectorIteratorIiE7isValidEv(ptr)

declare ptr @_ZN14VectorIteratorIiE3getEv(ptr)

declare void @_ZN14VectorIteratorIiE4nextEv(ptr)

declare void @_ZN14VectorIteratorIiE6getIdxEv(ptr dead_on_unwind noalias writable sret(%struct.Pair) align 8, ptr)

declare ptr @_ZN4PairImRiE8getFirstEv(ptr)

declare ptr @_ZN4PairImRiE9getSecondEv(ptr)

declare void @_Z13op.minusequalIiiEvR14VectorIteratorIiEi(ptr, i32)

declare void @_Z16op.plusplus.postIiEvR14VectorIteratorIiE(ptr)

declare void @_Z18op.minusminus.postIiEvR14VectorIteratorIiE(ptr)

declare void @_Z12op.plusequalIiiEvR14VectorIteratorIiEi(ptr, i32)

declare ptr @_ZN6VectorIiE3getEj(ptr, i32)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

declare void @_ZN6VectorIiE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32))

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree }
attributes #2 = { nounwind }
attributes #3 = { cold noreturn nounwind }
attributes #4 = { nofree nounwind }

!llvm.module.flags = !{!8, !9, !10, !11, !12, !13}
!llvm.ident = !{!14}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !3, line: 70, type: !5, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev [self-hosted] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false)
!3 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-dbg-info-complex")
!4 = !{!0}
!5 = !DICompositeType(tag: DW_TAG_array_type, baseType: !6, size: 192, elements: !7)
!6 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!7 = !{}
!8 = !{i32 8, !"PIC Level", i32 2}
!9 = !{i32 7, !"PIE Level", i32 2}
!10 = !{i32 7, !"uwtable", i32 2}
!11 = !{i32 7, !"frame-pointer", i32 0}
!12 = !{i32 7, !"Dwarf Version", i32 5}
!13 = !{i32 2, !"Debug Info Version", i32 3}
!14 = !{!"spice version dev [self-hosted] (https://github.com/spicelang/spice)"}
!15 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainiPPKc", scope: !3, file: !3, line: 6, type: !16, scopeLine: 6, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !21)
!16 = !DISubroutineType(types: !17)
!17 = !{!18, !18, !19}
!18 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!19 = !DICompositeType(tag: DW_TAG_array_type, baseType: !20, elements: !7)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !6, size: 64, dwarfAddressSpace: 0)
!21 = !{!22, !23, !24, !32, !38, !44, !46, !48, !51}
!22 = !DILocalVariable(name: "_argc", arg: 1, scope: !15, file: !3, line: 6, type: !18)
!23 = !DILocalVariable(name: "_argv", arg: 2, scope: !15, file: !3, line: 6, type: !19)
!24 = !DILocalVariable(name: "vi", scope: !15, file: !3, line: 8, type: !25)
!25 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector<int>", scope: !3, file: !3, line: 29, size: 256, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !26, identifier: "struct.Vector")
!26 = !{!27, !29, !31}
!27 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !25, file: !3, line: 30, baseType: !28, size: 64, offset: 64)
!28 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64, dwarfAddressSpace: 0)
!29 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !25, file: !3, line: 31, baseType: !30, size: 64, offset: 128)
!30 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!31 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !25, file: !3, line: 32, baseType: !30, size: 64, offset: 192)
!32 = !DILocalVariable(name: "it", scope: !15, file: !3, line: 15, type: !33)
!33 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator<int>", scope: !3, file: !3, line: 445, size: 192, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !34, identifier: "struct.VectorIterator")
!34 = !{!35, !37}
!35 = !DIDerivedType(tag: DW_TAG_member, name: "vector", scope: !33, file: !3, line: 446, baseType: !36, offset: 64)
!36 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !25)
!37 = !DIDerivedType(tag: DW_TAG_member, name: "cursor", scope: !33, file: !3, line: 447, baseType: !30, size: 64, offset: 128)
!38 = !DILocalVariable(name: "pair", scope: !15, file: !3, line: 23, type: !39)
!39 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair<unsigned long,int&>", scope: !3, file: !3, line: 8, size: 128, align: 64, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !40, identifier: "struct.Pair")
!40 = !{!41, !42}
!41 = !DIDerivedType(tag: DW_TAG_member, name: "first", scope: !39, file: !3, line: 9, baseType: !30, size: 64)
!42 = !DIDerivedType(tag: DW_TAG_member, name: "second", scope: !39, file: !3, line: 10, baseType: !43, offset: 64)
!43 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !18)
!44 = !DILocalVariable(name: "item", scope: !45, file: !3, line: 48, type: !18)
!45 = distinct !DILexicalBlock(scope: !15, file: !3, line: 48, column: 5)
!46 = !DILocalVariable(name: "item", scope: !47, file: !3, line: 56, type: !43)
!47 = distinct !DILexicalBlock(scope: !15, file: !3, line: 56, column: 5)
!48 = !DILocalVariable(name: "idx", scope: !49, file: !3, line: 63, type: !50)
!49 = distinct !DILexicalBlock(scope: !15, file: !3, line: 63, column: 5)
!50 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!51 = !DILocalVariable(name: "item", scope: !49, file: !3, line: 63, type: !43)
!52 = !DILocation(line: 6, column: 1, scope: !15)
!53 = !DILocation(line: 8, column: 22, scope: !15)
!54 = !DILocation(line: 9, column: 17, scope: !15)
!55 = !DILocation(line: 10, column: 17, scope: !15)
!56 = !DILocation(line: 11, column: 17, scope: !15)
!57 = !DILocation(line: 12, column: 12, scope: !15)
!58 = !DILocation(line: 12, column: 28, scope: !15)
!59 = !{!"branch_weights", i32 1048575, i32 1}
!60 = !DILocation(line: 15, column: 14, scope: !15)
!61 = !DILocation(line: 16, column: 12, scope: !15)
!62 = !DILocation(line: 17, column: 12, scope: !15)
!63 = !DILocation(line: 17, column: 24, scope: !15)
!64 = !DILocation(line: 18, column: 12, scope: !15)
!65 = !DILocation(line: 18, column: 24, scope: !15)
!66 = !DILocation(line: 19, column: 5, scope: !15)
!67 = !DILocation(line: 20, column: 12, scope: !15)
!68 = !DILocation(line: 20, column: 24, scope: !15)
!69 = !DILocation(line: 21, column: 12, scope: !15)
!70 = !DILocation(line: 22, column: 5, scope: !15)
!71 = !DILocation(line: 23, column: 16, scope: !15)
!72 = !DILocation(line: 24, column: 12, scope: !15)
!73 = !DILocation(line: 24, column: 31, scope: !15)
!74 = !DILocation(line: 25, column: 12, scope: !15)
!75 = !DILocation(line: 25, column: 32, scope: !15)
!76 = !DILocation(line: 26, column: 5, scope: !15)
!77 = !DILocation(line: 27, column: 13, scope: !15)
!78 = !DILocation(line: 30, column: 17, scope: !15)
!79 = !DILocation(line: 31, column: 17, scope: !15)
!80 = !DILocation(line: 32, column: 12, scope: !15)
!81 = !DILocation(line: 35, column: 5, scope: !15)
!82 = !DILocation(line: 36, column: 12, scope: !15)
!83 = !DILocation(line: 36, column: 24, scope: !15)
!84 = !DILocation(line: 37, column: 12, scope: !15)
!85 = !DILocation(line: 38, column: 5, scope: !15)
!86 = !DILocation(line: 39, column: 12, scope: !15)
!87 = !DILocation(line: 39, column: 24, scope: !15)
!88 = !DILocation(line: 40, column: 5, scope: !15)
!89 = !DILocation(line: 41, column: 12, scope: !15)
!90 = !DILocation(line: 41, column: 24, scope: !15)
!91 = !DILocation(line: 42, column: 5, scope: !15)
!92 = !DILocation(line: 43, column: 12, scope: !15)
!93 = !DILocation(line: 43, column: 24, scope: !15)
!94 = !DILocation(line: 44, column: 5, scope: !15)
!95 = !DILocation(line: 45, column: 13, scope: !15)
!96 = !DILocation(line: 48, column: 24, scope: !45)
!97 = !DILocation(line: 48, column: 5, scope: !45)
!98 = !DILocation(line: 49, column: 9, scope: !45)
!99 = !DILocation(line: 50, column: 5, scope: !45)
!100 = !DILocation(line: 51, column: 19, scope: !15)
!101 = !DILocation(line: 51, column: 25, scope: !15)
!102 = !DILocation(line: 52, column: 19, scope: !15)
!103 = !DILocation(line: 52, column: 25, scope: !15)
!104 = !DILocation(line: 53, column: 19, scope: !15)
!105 = !DILocation(line: 53, column: 25, scope: !15)
!106 = !DILocation(line: 56, column: 25, scope: !47)
!107 = !DILocation(line: 56, column: 5, scope: !47)
!108 = !DILocation(line: 57, column: 9, scope: !47)
!109 = !DILocation(line: 58, column: 5, scope: !47)
!110 = !DILocation(line: 59, column: 19, scope: !15)
!111 = !DILocation(line: 59, column: 25, scope: !15)
!112 = !DILocation(line: 60, column: 19, scope: !15)
!113 = !DILocation(line: 60, column: 25, scope: !15)
!114 = !DILocation(line: 61, column: 19, scope: !15)
!115 = !DILocation(line: 61, column: 25, scope: !15)
!116 = !DILocation(line: 63, column: 35, scope: !49)
!117 = !DILocation(line: 63, column: 5, scope: !49)
!118 = !DILocation(line: 64, column: 9, scope: !49)
!119 = !DILocation(line: 65, column: 5, scope: !49)
!120 = !DILocation(line: 66, column: 19, scope: !15)
!121 = !DILocation(line: 66, column: 25, scope: !15)
!122 = !DILocation(line: 67, column: 19, scope: !15)
!123 = !DILocation(line: 67, column: 25, scope: !15)
!124 = !DILocation(line: 68, column: 19, scope: !15)
!125 = !DILocation(line: 68, column: 25, scope: !15)
!126 = !DILocation(line: 70, column: 5, scope: !15)
!127 = !DILocation(line: 71, column: 1, scope: !15)
