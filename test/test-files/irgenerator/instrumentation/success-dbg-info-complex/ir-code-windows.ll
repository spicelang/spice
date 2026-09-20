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
    #dbg_declare(ptr %_argc, !22, !DIExpression(), !23)
  store i32 %0, ptr %_argc, align 4, !dbg !23
    #dbg_declare(ptr %_argv, !24, !DIExpression(), !23)
  store ptr %1, ptr %_argv, align 8, !dbg !23
  call void @_ZN6VectorIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !25
    #dbg_declare(ptr %vi, !26, !DIExpression(), !25)
  store i32 123, ptr %3, align 4, !dbg !36
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %3), !dbg !36
  store i32 4321, ptr %4, align 4, !dbg !37
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %4), !dbg !37
  store i32 9876, ptr %5, align 4, !dbg !38
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %5), !dbg !38
  %13 = call noundef i64 @_ZN6VectorIiE7getSizeEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !39
  %14 = icmp eq i64 %13, 3, !dbg !40
  br i1 %14, label %assert.exit.L12, label %assert.then.L12, !dbg !40, !prof !41

assert.then.L12:                                  ; preds = %2
  %15 = call ptr @__acrt_iob_func(i32 2), !dbg !40
  %16 = call i32 (ptr, ptr, ...) @fprintf(ptr %15, ptr @anon.string.0), !dbg !40
  call void @exit(i32 1), !dbg !40
  unreachable, !dbg !40

assert.exit.L12:                                  ; preds = %2
  %17 = call noundef %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !42
  store %struct.VectorIterator %17, ptr %it, align 8, !dbg !42
    #dbg_declare(ptr %it, !43, !DIExpression(), !42)
  %18 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !49
  br i1 %18, label %assert.exit.L16, label %assert.then.L16, !dbg !49, !prof !41

assert.then.L16:                                  ; preds = %assert.exit.L12
  %19 = call ptr @__acrt_iob_func(i32 2), !dbg !49
  %20 = call i32 (ptr, ptr, ...) @fprintf(ptr %19, ptr @anon.string.1), !dbg !49
  call void @exit(i32 1), !dbg !49
  unreachable, !dbg !49

assert.exit.L16:                                  ; preds = %assert.exit.L12
  %21 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !50
  %22 = load i32, ptr %21, align 4, !dbg !51
  %23 = icmp eq i32 %22, 123, !dbg !51
  br i1 %23, label %assert.exit.L17, label %assert.then.L17, !dbg !51, !prof !41

assert.then.L17:                                  ; preds = %assert.exit.L16
  %24 = call ptr @__acrt_iob_func(i32 2), !dbg !51
  %25 = call i32 (ptr, ptr, ...) @fprintf(ptr %24, ptr @anon.string.2), !dbg !51
  call void @exit(i32 1), !dbg !51
  unreachable, !dbg !51

assert.exit.L17:                                  ; preds = %assert.exit.L16
  %26 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !52
  %27 = load i32, ptr %26, align 4, !dbg !53
  %28 = icmp eq i32 %27, 123, !dbg !53
  br i1 %28, label %assert.exit.L18, label %assert.then.L18, !dbg !53, !prof !41

assert.then.L18:                                  ; preds = %assert.exit.L17
  %29 = call ptr @__acrt_iob_func(i32 2), !dbg !53
  %30 = call i32 (ptr, ptr, ...) @fprintf(ptr %29, ptr @anon.string.3), !dbg !53
  call void @exit(i32 1), !dbg !53
  unreachable, !dbg !53

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !54
  %31 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !55
  %32 = load i32, ptr %31, align 4, !dbg !56
  %33 = icmp eq i32 %32, 4321, !dbg !56
  br i1 %33, label %assert.exit.L20, label %assert.then.L20, !dbg !56, !prof !41

assert.then.L20:                                  ; preds = %assert.exit.L18
  %34 = call ptr @__acrt_iob_func(i32 2), !dbg !56
  %35 = call i32 (ptr, ptr, ...) @fprintf(ptr %34, ptr @anon.string.4), !dbg !56
  call void @exit(i32 1), !dbg !56
  unreachable, !dbg !56

assert.exit.L20:                                  ; preds = %assert.exit.L18
  %36 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !57
  br i1 %36, label %assert.exit.L21, label %assert.then.L21, !dbg !57, !prof !41

assert.then.L21:                                  ; preds = %assert.exit.L20
  %37 = call ptr @__acrt_iob_func(i32 2), !dbg !57
  %38 = call i32 (ptr, ptr, ...) @fprintf(ptr %37, ptr @anon.string.5), !dbg !57
  call void @exit(i32 1), !dbg !57
  unreachable, !dbg !57

assert.exit.L21:                                  ; preds = %assert.exit.L20
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !58
  %39 = call noundef %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !59
  store %struct.Pair %39, ptr %pair, align 8, !dbg !59
    #dbg_declare(ptr %pair, !60, !DIExpression(), !59)
  %40 = call noundef ptr @_ZN4PairImRiE8getFirstEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !69
  %41 = load i64, ptr %40, align 8, !dbg !70
  %42 = icmp eq i64 %41, 2, !dbg !70
  br i1 %42, label %assert.exit.L24, label %assert.then.L24, !dbg !70, !prof !41

assert.then.L24:                                  ; preds = %assert.exit.L21
  %43 = call ptr @__acrt_iob_func(i32 2), !dbg !70
  %44 = call i32 (ptr, ptr, ...) @fprintf(ptr %43, ptr @anon.string.6), !dbg !70
  call void @exit(i32 1), !dbg !70
  unreachable, !dbg !70

assert.exit.L24:                                  ; preds = %assert.exit.L21
  %45 = call noundef ptr @_ZN4PairImRiE9getSecondEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !71
  %46 = load i32, ptr %45, align 4, !dbg !72
  %47 = icmp eq i32 %46, 9876, !dbg !72
  br i1 %47, label %assert.exit.L25, label %assert.then.L25, !dbg !72, !prof !41

assert.then.L25:                                  ; preds = %assert.exit.L24
  %48 = call ptr @__acrt_iob_func(i32 2), !dbg !72
  %49 = call i32 (ptr, ptr, ...) @fprintf(ptr %48, ptr @anon.string.7), !dbg !72
  call void @exit(i32 1), !dbg !72
  unreachable, !dbg !72

assert.exit.L25:                                  ; preds = %assert.exit.L24
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !73
  %50 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !74
  %51 = xor i1 %50, true, !dbg !74
  br i1 %51, label %assert.exit.L27, label %assert.then.L27, !dbg !74, !prof !41

assert.then.L27:                                  ; preds = %assert.exit.L25
  %52 = call ptr @__acrt_iob_func(i32 2), !dbg !74
  %53 = call i32 (ptr, ptr, ...) @fprintf(ptr %52, ptr @anon.string.8), !dbg !74
  call void @exit(i32 1), !dbg !74
  unreachable, !dbg !74

assert.exit.L27:                                  ; preds = %assert.exit.L25
  store i32 321, ptr %6, align 4, !dbg !75
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %6), !dbg !75
  store i32 -99, ptr %7, align 4, !dbg !76
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr noundef %7), !dbg !76
  %54 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !77
  br i1 %54, label %assert.exit.L32, label %assert.then.L32, !dbg !77, !prof !41

assert.then.L32:                                  ; preds = %assert.exit.L27
  %55 = call ptr @__acrt_iob_func(i32 2), !dbg !77
  %56 = call i32 (ptr, ptr, ...) @fprintf(ptr %55, ptr @anon.string.9), !dbg !77
  call void @exit(i32 1), !dbg !77
  unreachable, !dbg !77

assert.exit.L32:                                  ; preds = %assert.exit.L27
  call void @_Z13op.minusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 3), !dbg !78
  %57 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !79
  %58 = load i32, ptr %57, align 4, !dbg !80
  %59 = icmp eq i32 %58, 123, !dbg !80
  br i1 %59, label %assert.exit.L36, label %assert.then.L36, !dbg !80, !prof !41

assert.then.L36:                                  ; preds = %assert.exit.L32
  %60 = call ptr @__acrt_iob_func(i32 2), !dbg !80
  %61 = call i32 (ptr, ptr, ...) @fprintf(ptr %60, ptr @anon.string.10), !dbg !80
  call void @exit(i32 1), !dbg !80
  unreachable, !dbg !80

assert.exit.L36:                                  ; preds = %assert.exit.L32
  %62 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !81
  br i1 %62, label %assert.exit.L37, label %assert.then.L37, !dbg !81, !prof !41

assert.then.L37:                                  ; preds = %assert.exit.L36
  %63 = call ptr @__acrt_iob_func(i32 2), !dbg !81
  %64 = call i32 (ptr, ptr, ...) @fprintf(ptr %63, ptr @anon.string.11), !dbg !81
  call void @exit(i32 1), !dbg !81
  unreachable, !dbg !81

assert.exit.L37:                                  ; preds = %assert.exit.L36
  %65 = load %struct.VectorIterator, ptr %it, align 8, !dbg !82
  call void @_Z16op.plusplus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !82
  %66 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !83
  %67 = load i32, ptr %66, align 4, !dbg !84
  %68 = icmp eq i32 %67, 4321, !dbg !84
  br i1 %68, label %assert.exit.L39, label %assert.then.L39, !dbg !84, !prof !41

assert.then.L39:                                  ; preds = %assert.exit.L37
  %69 = call ptr @__acrt_iob_func(i32 2), !dbg !84
  %70 = call i32 (ptr, ptr, ...) @fprintf(ptr %69, ptr @anon.string.12), !dbg !84
  call void @exit(i32 1), !dbg !84
  unreachable, !dbg !84

assert.exit.L39:                                  ; preds = %assert.exit.L37
  %71 = load %struct.VectorIterator, ptr %it, align 8, !dbg !85
  call void @_Z18op.minusminus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !85
  %72 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !86
  %73 = load i32, ptr %72, align 4, !dbg !87
  %74 = icmp eq i32 %73, 123, !dbg !87
  br i1 %74, label %assert.exit.L41, label %assert.then.L41, !dbg !87, !prof !41

assert.then.L41:                                  ; preds = %assert.exit.L39
  %75 = call ptr @__acrt_iob_func(i32 2), !dbg !87
  %76 = call i32 (ptr, ptr, ...) @fprintf(ptr %75, ptr @anon.string.13), !dbg !87
  call void @exit(i32 1), !dbg !87
  unreachable, !dbg !87

assert.exit.L41:                                  ; preds = %assert.exit.L39
  call void @_Z12op.plusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 4), !dbg !88
  %77 = call noundef ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !89
  %78 = load i32, ptr %77, align 4, !dbg !90
  %79 = icmp eq i32 %78, -99, !dbg !90
  br i1 %79, label %assert.exit.L43, label %assert.then.L43, !dbg !90, !prof !41

assert.then.L43:                                  ; preds = %assert.exit.L41
  %80 = call ptr @__acrt_iob_func(i32 2), !dbg !90
  %81 = call i32 (ptr, ptr, ...) @fprintf(ptr %80, ptr @anon.string.14), !dbg !90
  call void @exit(i32 1), !dbg !90
  unreachable, !dbg !90

assert.exit.L43:                                  ; preds = %assert.exit.L41
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !91
  %82 = call noundef zeroext i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !92
  %83 = xor i1 %82, true, !dbg !92
  br i1 %83, label %assert.exit.L45, label %assert.then.L45, !dbg !92, !prof !41

assert.then.L45:                                  ; preds = %assert.exit.L43
  %84 = call ptr @__acrt_iob_func(i32 2), !dbg !92
  %85 = call i32 (ptr, ptr, ...) @fprintf(ptr %84, ptr @anon.string.15), !dbg !92
  call void @exit(i32 1), !dbg !92
  unreachable, !dbg !92

assert.exit.L45:                                  ; preds = %assert.exit.L43
  %86 = call noundef %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !93
  store %struct.VectorIterator %86, ptr %8, align 8, !dbg !93
    #dbg_declare(ptr %item, !95, !DIExpression(), !93)
  br label %foreach.head.L48, !dbg !93

foreach.head.L48:                                 ; preds = %foreach.tail.L48, %assert.exit.L45
  %87 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %8), !dbg !96
  br i1 %87, label %foreach.body.L48, label %foreach.exit.L48, !dbg !96

foreach.body.L48:                                 ; preds = %foreach.head.L48
  %88 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %8), !dbg !96
  %89 = load i32, ptr %88, align 4, !dbg !96
  store i32 %89, ptr %item, align 4, !dbg !96
  %90 = load i32, ptr %item, align 4, !dbg !97
  %91 = add nsw i32 %90, 1, !dbg !97
  store i32 %91, ptr %item, align 4, !dbg !97
  br label %foreach.tail.L48, !dbg !98

foreach.tail.L48:                                 ; preds = %foreach.body.L48
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %8), !dbg !96
  br label %foreach.head.L48, !dbg !96

foreach.exit.L48:                                 ; preds = %foreach.head.L48
  %92 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !99
  %93 = load i32, ptr %92, align 4, !dbg !100
  %94 = icmp eq i32 %93, 123, !dbg !100
  br i1 %94, label %assert.exit.L51, label %assert.then.L51, !dbg !100, !prof !41

assert.then.L51:                                  ; preds = %foreach.exit.L48
  %95 = call ptr @__acrt_iob_func(i32 2), !dbg !100
  %96 = call i32 (ptr, ptr, ...) @fprintf(ptr %95, ptr @anon.string.16), !dbg !100
  call void @exit(i32 1), !dbg !100
  unreachable, !dbg !100

assert.exit.L51:                                  ; preds = %foreach.exit.L48
  %97 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !101
  %98 = load i32, ptr %97, align 4, !dbg !102
  %99 = icmp eq i32 %98, 4321, !dbg !102
  br i1 %99, label %assert.exit.L52, label %assert.then.L52, !dbg !102, !prof !41

assert.then.L52:                                  ; preds = %assert.exit.L51
  %100 = call ptr @__acrt_iob_func(i32 2), !dbg !102
  %101 = call i32 (ptr, ptr, ...) @fprintf(ptr %100, ptr @anon.string.17), !dbg !102
  call void @exit(i32 1), !dbg !102
  unreachable, !dbg !102

assert.exit.L52:                                  ; preds = %assert.exit.L51
  %102 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !103
  %103 = load i32, ptr %102, align 4, !dbg !104
  %104 = icmp eq i32 %103, 9876, !dbg !104
  br i1 %104, label %assert.exit.L53, label %assert.then.L53, !dbg !104, !prof !41

assert.then.L53:                                  ; preds = %assert.exit.L52
  %105 = call ptr @__acrt_iob_func(i32 2), !dbg !104
  %106 = call i32 (ptr, ptr, ...) @fprintf(ptr %105, ptr @anon.string.18), !dbg !104
  call void @exit(i32 1), !dbg !104
  unreachable, !dbg !104

assert.exit.L53:                                  ; preds = %assert.exit.L52
  %107 = call noundef %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !105
  store %struct.VectorIterator %107, ptr %9, align 8, !dbg !105
    #dbg_declare(ptr %item1, !107, !DIExpression(), !105)
  br label %foreach.head.L56, !dbg !105

foreach.head.L56:                                 ; preds = %foreach.tail.L56, %assert.exit.L53
  %108 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %9), !dbg !108
  br i1 %108, label %foreach.body.L56, label %foreach.exit.L56, !dbg !108

foreach.body.L56:                                 ; preds = %foreach.head.L56
  %109 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %9), !dbg !108
    #dbg_declare(ptr %10, !107, !DIExpression(), !108)
  store ptr %109, ptr %10, align 8, !dbg !108
  %110 = load ptr, ptr %10, align 8, !dbg !109
  %111 = load i32, ptr %110, align 4, !dbg !109
  %112 = add nsw i32 %111, 1, !dbg !109
  store i32 %112, ptr %110, align 4, !dbg !109
  br label %foreach.tail.L56, !dbg !110

foreach.tail.L56:                                 ; preds = %foreach.body.L56
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %9), !dbg !108
  br label %foreach.head.L56, !dbg !108

foreach.exit.L56:                                 ; preds = %foreach.head.L56
  %113 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !111
  %114 = load i32, ptr %113, align 4, !dbg !112
  %115 = icmp eq i32 %114, 124, !dbg !112
  br i1 %115, label %assert.exit.L59, label %assert.then.L59, !dbg !112, !prof !41

assert.then.L59:                                  ; preds = %foreach.exit.L56
  %116 = call ptr @__acrt_iob_func(i32 2), !dbg !112
  %117 = call i32 (ptr, ptr, ...) @fprintf(ptr %116, ptr @anon.string.19), !dbg !112
  call void @exit(i32 1), !dbg !112
  unreachable, !dbg !112

assert.exit.L59:                                  ; preds = %foreach.exit.L56
  %118 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !113
  %119 = load i32, ptr %118, align 4, !dbg !114
  %120 = icmp eq i32 %119, 4322, !dbg !114
  br i1 %120, label %assert.exit.L60, label %assert.then.L60, !dbg !114, !prof !41

assert.then.L60:                                  ; preds = %assert.exit.L59
  %121 = call ptr @__acrt_iob_func(i32 2), !dbg !114
  %122 = call i32 (ptr, ptr, ...) @fprintf(ptr %121, ptr @anon.string.20), !dbg !114
  call void @exit(i32 1), !dbg !114
  unreachable, !dbg !114

assert.exit.L60:                                  ; preds = %assert.exit.L59
  %123 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !115
  %124 = load i32, ptr %123, align 4, !dbg !116
  %125 = icmp eq i32 %124, 9877, !dbg !116
  br i1 %125, label %assert.exit.L61, label %assert.then.L61, !dbg !116, !prof !41

assert.then.L61:                                  ; preds = %assert.exit.L60
  %126 = call ptr @__acrt_iob_func(i32 2), !dbg !116
  %127 = call i32 (ptr, ptr, ...) @fprintf(ptr %126, ptr @anon.string.21), !dbg !116
  call void @exit(i32 1), !dbg !116
  unreachable, !dbg !116

assert.exit.L61:                                  ; preds = %assert.exit.L60
  %128 = call noundef %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !117
  store %struct.VectorIterator %128, ptr %11, align 8, !dbg !117
    #dbg_declare(ptr %idx, !119, !DIExpression(), !117)
  store i64 0, ptr %idx, align 8, !dbg !117
    #dbg_declare(ptr %item2, !121, !DIExpression(), !117)
  br label %foreach.head.L63, !dbg !117

foreach.head.L63:                                 ; preds = %foreach.tail.L63, %assert.exit.L61
  %129 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %11), !dbg !122
  br i1 %129, label %foreach.body.L63, label %foreach.exit.L63, !dbg !122

foreach.body.L63:                                 ; preds = %foreach.head.L63
  %pair3 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr %11), !dbg !122
  store %struct.Pair %pair3, ptr %pair.addr, align 8, !dbg !122
  %130 = load i64, ptr %pair.addr, align 8, !dbg !122
  store i64 %130, ptr %idx, align 8, !dbg !122
  %item.addr = getelementptr inbounds nuw %struct.Pair, ptr %pair.addr, i32 0, i32 1, !dbg !122
    #dbg_declare(ptr %12, !121, !DIExpression(), !122)
  %131 = load ptr, ptr %item.addr, align 8, !dbg !122
  store ptr %131, ptr %12, align 8, !dbg !122
  %132 = load i64, ptr %idx, align 8, !dbg !123
  %133 = trunc i64 %132 to i32, !dbg !123
  %134 = load ptr, ptr %12, align 8, !dbg !123
  %135 = load i32, ptr %134, align 4, !dbg !123
  %136 = add nsw i32 %135, %133, !dbg !123
  store i32 %136, ptr %134, align 4, !dbg !123
  br label %foreach.tail.L63, !dbg !124

foreach.tail.L63:                                 ; preds = %foreach.body.L63
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %11), !dbg !122
  br label %foreach.head.L63, !dbg !122

foreach.exit.L63:                                 ; preds = %foreach.head.L63
  %137 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 0), !dbg !125
  %138 = load i32, ptr %137, align 4, !dbg !126
  %139 = icmp eq i32 %138, 124, !dbg !126
  br i1 %139, label %assert.exit.L66, label %assert.then.L66, !dbg !126, !prof !41

assert.then.L66:                                  ; preds = %foreach.exit.L63
  %140 = call ptr @__acrt_iob_func(i32 2), !dbg !126
  %141 = call i32 (ptr, ptr, ...) @fprintf(ptr %140, ptr @anon.string.22), !dbg !126
  call void @exit(i32 1), !dbg !126
  unreachable, !dbg !126

assert.exit.L66:                                  ; preds = %foreach.exit.L63
  %142 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 1), !dbg !127
  %143 = load i32, ptr %142, align 4, !dbg !128
  %144 = icmp eq i32 %143, 4323, !dbg !128
  br i1 %144, label %assert.exit.L67, label %assert.then.L67, !dbg !128, !prof !41

assert.then.L67:                                  ; preds = %assert.exit.L66
  %145 = call ptr @__acrt_iob_func(i32 2), !dbg !128
  %146 = call i32 (ptr, ptr, ...) @fprintf(ptr %145, ptr @anon.string.23), !dbg !128
  call void @exit(i32 1), !dbg !128
  unreachable, !dbg !128

assert.exit.L67:                                  ; preds = %assert.exit.L66
  %147 = call noundef ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 noundef 2), !dbg !129
  %148 = load i32, ptr %147, align 4, !dbg !130
  %149 = icmp eq i32 %148, 9879, !dbg !130
  br i1 %149, label %assert.exit.L68, label %assert.then.L68, !dbg !130, !prof !41

assert.then.L68:                                  ; preds = %assert.exit.L67
  %150 = call ptr @__acrt_iob_func(i32 2), !dbg !130
  %151 = call i32 (ptr, ptr, ...) @fprintf(ptr %150, ptr @anon.string.24), !dbg !130
  call void @exit(i32 1), !dbg !130
  unreachable, !dbg !130

assert.exit.L68:                                  ; preds = %assert.exit.L67
  %152 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !131
  call void @_ZN6VectorIiE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !132
  ret i32 0, !dbg !132
}

declare void @_ZN6VectorIiE4ctorEv(ptr)

declare void @_ZN6VectorIiE8pushBackERKi(ptr, ptr)

declare i64 @_ZN6VectorIiE7getSizeEv(ptr)

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #1

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) #2

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #3

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

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #4

declare void @_ZN6VectorIiE4dtorEv(ptr noundef nonnull align 8 dereferenceable(32))

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree }
attributes #2 = { nounwind }
attributes #3 = { cold noreturn nounwind }
attributes #4 = { nofree nounwind }

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
!10 = !{i32 7, !"frame-pointer", i32 0}
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
!22 = !DILocalVariable(name: "_argc", arg: 1, scope: !14, file: !5, line: 6, type: !17)
!23 = !DILocation(line: 6, column: 1, scope: !14)
!24 = !DILocalVariable(name: "_argv", arg: 2, scope: !14, file: !5, line: 6, type: !18)
!25 = !DILocation(line: 8, column: 22, scope: !14)
!26 = !DILocalVariable(name: "vi", scope: !14, file: !5, line: 8, type: !27)
!27 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector<int>", scope: !5, file: !5, line: 26, size: 256, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !28, templateParams: !34, identifier: "struct.Vector")
!28 = !{!29, !31, !33}
!29 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !27, file: !5, line: 27, baseType: !30, size: 64, offset: 64)
!30 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !17, size: 64)
!31 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !27, file: !5, line: 28, baseType: !32, size: 64, offset: 128)
!32 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!33 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !27, file: !5, line: 29, baseType: !32, size: 64, offset: 192)
!34 = !{!35}
!35 = !DITemplateTypeParameter(name: "T", type: !17)
!36 = !DILocation(line: 9, column: 17, scope: !14)
!37 = !DILocation(line: 10, column: 17, scope: !14)
!38 = !DILocation(line: 11, column: 17, scope: !14)
!39 = !DILocation(line: 12, column: 12, scope: !14)
!40 = !DILocation(line: 12, column: 28, scope: !14)
!41 = !{!"branch_weights", i32 1048575, i32 1}
!42 = !DILocation(line: 15, column: 14, scope: !14)
!43 = !DILocalVariable(name: "it", scope: !14, file: !5, line: 15, type: !44)
!44 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator<int>", scope: !5, file: !5, line: 423, size: 192, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !45, templateParams: !34, identifier: "struct.VectorIterator")
!45 = !{!46, !48}
!46 = !DIDerivedType(tag: DW_TAG_member, name: "vector", scope: !44, file: !5, line: 424, baseType: !47, size: 64, offset: 64)
!47 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !27, size: 64)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "cursor", scope: !44, file: !5, line: 425, baseType: !32, size: 64, offset: 128)
!49 = !DILocation(line: 16, column: 12, scope: !14)
!50 = !DILocation(line: 17, column: 12, scope: !14)
!51 = !DILocation(line: 17, column: 24, scope: !14)
!52 = !DILocation(line: 18, column: 12, scope: !14)
!53 = !DILocation(line: 18, column: 24, scope: !14)
!54 = !DILocation(line: 19, column: 5, scope: !14)
!55 = !DILocation(line: 20, column: 12, scope: !14)
!56 = !DILocation(line: 20, column: 24, scope: !14)
!57 = !DILocation(line: 21, column: 12, scope: !14)
!58 = !DILocation(line: 22, column: 5, scope: !14)
!59 = !DILocation(line: 23, column: 16, scope: !14)
!60 = !DILocalVariable(name: "pair", scope: !14, file: !5, line: 23, type: !61)
!61 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair<unsigned long,int&>", scope: !5, file: !5, line: 8, size: 128, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !62, templateParams: !66, identifier: "struct.Pair")
!62 = !{!63, !64}
!63 = !DIDerivedType(tag: DW_TAG_member, name: "first", scope: !61, file: !5, line: 9, baseType: !32, size: 64)
!64 = !DIDerivedType(tag: DW_TAG_member, name: "second", scope: !61, file: !5, line: 10, baseType: !65, size: 64, offset: 64)
!65 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !17, size: 64)
!66 = !{!67, !68}
!67 = !DITemplateTypeParameter(name: "V1", type: !32)
!68 = !DITemplateTypeParameter(name: "V2", type: !65)
!69 = !DILocation(line: 24, column: 12, scope: !14)
!70 = !DILocation(line: 24, column: 31, scope: !14)
!71 = !DILocation(line: 25, column: 12, scope: !14)
!72 = !DILocation(line: 25, column: 32, scope: !14)
!73 = !DILocation(line: 26, column: 5, scope: !14)
!74 = !DILocation(line: 27, column: 13, scope: !14)
!75 = !DILocation(line: 30, column: 17, scope: !14)
!76 = !DILocation(line: 31, column: 17, scope: !14)
!77 = !DILocation(line: 32, column: 12, scope: !14)
!78 = !DILocation(line: 35, column: 5, scope: !14)
!79 = !DILocation(line: 36, column: 12, scope: !14)
!80 = !DILocation(line: 36, column: 24, scope: !14)
!81 = !DILocation(line: 37, column: 12, scope: !14)
!82 = !DILocation(line: 38, column: 5, scope: !14)
!83 = !DILocation(line: 39, column: 12, scope: !14)
!84 = !DILocation(line: 39, column: 24, scope: !14)
!85 = !DILocation(line: 40, column: 5, scope: !14)
!86 = !DILocation(line: 41, column: 12, scope: !14)
!87 = !DILocation(line: 41, column: 24, scope: !14)
!88 = !DILocation(line: 42, column: 5, scope: !14)
!89 = !DILocation(line: 43, column: 12, scope: !14)
!90 = !DILocation(line: 43, column: 24, scope: !14)
!91 = !DILocation(line: 44, column: 5, scope: !14)
!92 = !DILocation(line: 45, column: 13, scope: !14)
!93 = !DILocation(line: 48, column: 24, scope: !94)
!94 = distinct !DILexicalBlock(scope: !14, file: !5, line: 48, column: 5)
!95 = !DILocalVariable(name: "item", scope: !94, file: !5, line: 48, type: !17)
!96 = !DILocation(line: 48, column: 5, scope: !94)
!97 = !DILocation(line: 49, column: 9, scope: !94)
!98 = !DILocation(line: 50, column: 5, scope: !94)
!99 = !DILocation(line: 51, column: 19, scope: !14)
!100 = !DILocation(line: 51, column: 25, scope: !14)
!101 = !DILocation(line: 52, column: 19, scope: !14)
!102 = !DILocation(line: 52, column: 25, scope: !14)
!103 = !DILocation(line: 53, column: 19, scope: !14)
!104 = !DILocation(line: 53, column: 25, scope: !14)
!105 = !DILocation(line: 56, column: 25, scope: !106)
!106 = distinct !DILexicalBlock(scope: !14, file: !5, line: 56, column: 5)
!107 = !DILocalVariable(name: "item", scope: !106, file: !5, line: 56, type: !65)
!108 = !DILocation(line: 56, column: 5, scope: !106)
!109 = !DILocation(line: 57, column: 9, scope: !106)
!110 = !DILocation(line: 58, column: 5, scope: !106)
!111 = !DILocation(line: 59, column: 19, scope: !14)
!112 = !DILocation(line: 59, column: 25, scope: !14)
!113 = !DILocation(line: 60, column: 19, scope: !14)
!114 = !DILocation(line: 60, column: 25, scope: !14)
!115 = !DILocation(line: 61, column: 19, scope: !14)
!116 = !DILocation(line: 61, column: 25, scope: !14)
!117 = !DILocation(line: 63, column: 35, scope: !118)
!118 = distinct !DILexicalBlock(scope: !14, file: !5, line: 63, column: 5)
!119 = !DILocalVariable(name: "idx", scope: !118, file: !5, line: 63, type: !120)
!120 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!121 = !DILocalVariable(name: "item", scope: !118, file: !5, line: 63, type: !65)
!122 = !DILocation(line: 63, column: 5, scope: !118)
!123 = !DILocation(line: 64, column: 9, scope: !118)
!124 = !DILocation(line: 65, column: 5, scope: !118)
!125 = !DILocation(line: 66, column: 19, scope: !14)
!126 = !DILocation(line: 66, column: 25, scope: !14)
!127 = !DILocation(line: 67, column: 19, scope: !14)
!128 = !DILocation(line: 67, column: 25, scope: !14)
!129 = !DILocation(line: 68, column: 19, scope: !14)
!130 = !DILocation(line: 68, column: 25, scope: !14)
!131 = !DILocation(line: 70, column: 5, scope: !14)
!132 = !DILocation(line: 71, column: 1, scope: !14)
