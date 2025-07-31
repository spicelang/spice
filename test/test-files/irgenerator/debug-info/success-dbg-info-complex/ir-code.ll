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

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) #0 !dbg !14 {
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
  store i32 %0, ptr %_argc, align 4, !dbg !23
    #dbg_declare(ptr %_argc, !24, !DIExpression(), !23)
  store ptr %1, ptr %_argv, align 8, !dbg !23
    #dbg_declare(ptr %_argv, !25, !DIExpression(), !23)
  call void @_ZN6VectorIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !26
    #dbg_declare(ptr %vi, !27, !DIExpression(), !35)
  store i32 123, ptr %3, align 4, !dbg !36
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %3), !dbg !36
  store i32 4321, ptr %4, align 4, !dbg !37
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %4), !dbg !37
  store i32 9876, ptr %5, align 4, !dbg !38
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %5), !dbg !38
  %13 = call i64 @_ZN6VectorIiE7getSizeEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !39
  %14 = icmp eq i64 %13, 3, !dbg !40
  br i1 %14, label %assert.exit.L10, label %assert.then.L10, !dbg !40, !prof !41

assert.then.L10:                                  ; preds = %2
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !40
  call void @exit(i32 1), !dbg !40
  unreachable, !dbg !40

assert.exit.L10:                                  ; preds = %2
  %16 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !42
  store %struct.VectorIterator %16, ptr %it, align 8, !dbg !42
    #dbg_declare(ptr %it, !43, !DIExpression(), !49)
  %17 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !50
  br i1 %17, label %assert.exit.L14, label %assert.then.L14, !dbg !50, !prof !41

assert.then.L14:                                  ; preds = %assert.exit.L10
  %18 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !50
  call void @exit(i32 1), !dbg !50
  unreachable, !dbg !50

assert.exit.L14:                                  ; preds = %assert.exit.L10
  %19 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !51
  %20 = load i32, ptr %19, align 4, !dbg !52
  %21 = icmp eq i32 %20, 123, !dbg !52
  br i1 %21, label %assert.exit.L15, label %assert.then.L15, !dbg !52, !prof !41

assert.then.L15:                                  ; preds = %assert.exit.L14
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !52
  call void @exit(i32 1), !dbg !52
  unreachable, !dbg !52

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %23 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !53
  %24 = load i32, ptr %23, align 4, !dbg !54
  %25 = icmp eq i32 %24, 123, !dbg !54
  br i1 %25, label %assert.exit.L16, label %assert.then.L16, !dbg !54, !prof !41

assert.then.L16:                                  ; preds = %assert.exit.L15
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !54
  call void @exit(i32 1), !dbg !54
  unreachable, !dbg !54

assert.exit.L16:                                  ; preds = %assert.exit.L15
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !55
  %27 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !56
  %28 = load i32, ptr %27, align 4, !dbg !57
  %29 = icmp eq i32 %28, 4321, !dbg !57
  br i1 %29, label %assert.exit.L18, label %assert.then.L18, !dbg !57, !prof !41

assert.then.L18:                                  ; preds = %assert.exit.L16
  %30 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !57
  call void @exit(i32 1), !dbg !57
  unreachable, !dbg !57

assert.exit.L18:                                  ; preds = %assert.exit.L16
  %31 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !58
  br i1 %31, label %assert.exit.L19, label %assert.then.L19, !dbg !58, !prof !41

assert.then.L19:                                  ; preds = %assert.exit.L18
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !58
  call void @exit(i32 1), !dbg !58
  unreachable, !dbg !58

assert.exit.L19:                                  ; preds = %assert.exit.L18
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !59
  %33 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !60
  store %struct.Pair %33, ptr %pair, align 8, !dbg !60
    #dbg_declare(ptr %pair, !61, !DIExpression(), !67)
  %34 = call ptr @_ZN4PairImRiE8getFirstEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !68
  %35 = load i64, ptr %34, align 8, !dbg !69
  %36 = icmp eq i64 %35, 2, !dbg !69
  br i1 %36, label %assert.exit.L22, label %assert.then.L22, !dbg !69, !prof !41

assert.then.L22:                                  ; preds = %assert.exit.L19
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !69
  call void @exit(i32 1), !dbg !69
  unreachable, !dbg !69

assert.exit.L22:                                  ; preds = %assert.exit.L19
  %38 = call ptr @_ZN4PairImRiE9getSecondEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !70
  %39 = load i32, ptr %38, align 4, !dbg !71
  %40 = icmp eq i32 %39, 9876, !dbg !71
  br i1 %40, label %assert.exit.L23, label %assert.then.L23, !dbg !71, !prof !41

assert.then.L23:                                  ; preds = %assert.exit.L22
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !71
  call void @exit(i32 1), !dbg !71
  unreachable, !dbg !71

assert.exit.L23:                                  ; preds = %assert.exit.L22
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !72
  %42 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !73
  %43 = xor i1 %42, true, !dbg !73
  br i1 %43, label %assert.exit.L25, label %assert.then.L25, !dbg !73, !prof !41

assert.then.L25:                                  ; preds = %assert.exit.L23
  %44 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !73
  call void @exit(i32 1), !dbg !73
  unreachable, !dbg !73

assert.exit.L25:                                  ; preds = %assert.exit.L23
  store i32 321, ptr %6, align 4, !dbg !74
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %6), !dbg !74
  store i32 -99, ptr %7, align 4, !dbg !75
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %7), !dbg !75
  %45 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !76
  br i1 %45, label %assert.exit.L30, label %assert.then.L30, !dbg !76, !prof !41

assert.then.L30:                                  ; preds = %assert.exit.L25
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !76
  call void @exit(i32 1), !dbg !76
  unreachable, !dbg !76

assert.exit.L30:                                  ; preds = %assert.exit.L25
  call void @_Z13op.minusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 3), !dbg !77
  %47 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !78
  %48 = load i32, ptr %47, align 4, !dbg !79
  %49 = icmp eq i32 %48, 123, !dbg !79
  br i1 %49, label %assert.exit.L34, label %assert.then.L34, !dbg !79, !prof !41

assert.then.L34:                                  ; preds = %assert.exit.L30
  %50 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !79
  call void @exit(i32 1), !dbg !79
  unreachable, !dbg !79

assert.exit.L34:                                  ; preds = %assert.exit.L30
  %51 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !80
  br i1 %51, label %assert.exit.L35, label %assert.then.L35, !dbg !80, !prof !41

assert.then.L35:                                  ; preds = %assert.exit.L34
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !80
  call void @exit(i32 1), !dbg !80
  unreachable, !dbg !80

assert.exit.L35:                                  ; preds = %assert.exit.L34
  %53 = load %struct.VectorIterator, ptr %it, align 8, !dbg !81
  call void @_Z16op.plusplus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !81
  %54 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !82
  %55 = load i32, ptr %54, align 4, !dbg !83
  %56 = icmp eq i32 %55, 4321, !dbg !83
  br i1 %56, label %assert.exit.L37, label %assert.then.L37, !dbg !83, !prof !41

assert.then.L37:                                  ; preds = %assert.exit.L35
  %57 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !83
  call void @exit(i32 1), !dbg !83
  unreachable, !dbg !83

assert.exit.L37:                                  ; preds = %assert.exit.L35
  %58 = load %struct.VectorIterator, ptr %it, align 8, !dbg !84
  call void @_Z18op.minusminus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !84
  %59 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !85
  %60 = load i32, ptr %59, align 4, !dbg !86
  %61 = icmp eq i32 %60, 123, !dbg !86
  br i1 %61, label %assert.exit.L39, label %assert.then.L39, !dbg !86, !prof !41

assert.then.L39:                                  ; preds = %assert.exit.L37
  %62 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !86
  call void @exit(i32 1), !dbg !86
  unreachable, !dbg !86

assert.exit.L39:                                  ; preds = %assert.exit.L37
  call void @_Z12op.plusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 4), !dbg !87
  %63 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !88
  %64 = load i32, ptr %63, align 4, !dbg !89
  %65 = icmp eq i32 %64, -99, !dbg !89
  br i1 %65, label %assert.exit.L41, label %assert.then.L41, !dbg !89, !prof !41

assert.then.L41:                                  ; preds = %assert.exit.L39
  %66 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !89
  call void @exit(i32 1), !dbg !89
  unreachable, !dbg !89

assert.exit.L41:                                  ; preds = %assert.exit.L39
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !90
  %67 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !91
  %68 = xor i1 %67, true, !dbg !91
  br i1 %68, label %assert.exit.L43, label %assert.then.L43, !dbg !91, !prof !41

assert.then.L43:                                  ; preds = %assert.exit.L41
  %69 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !91
  call void @exit(i32 1), !dbg !91
  unreachable, !dbg !91

assert.exit.L43:                                  ; preds = %assert.exit.L41
  %70 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !92
  store %struct.VectorIterator %70, ptr %8, align 8, !dbg !92
    #dbg_declare(ptr %item, !94, !DIExpression(), !95)
  br label %foreach.head.L46, !dbg !95

foreach.head.L46:                                 ; preds = %foreach.tail.L46, %assert.exit.L43
  %71 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %8), !dbg !95
  br i1 %71, label %foreach.body.L46, label %foreach.exit.L46, !dbg !95

foreach.body.L46:                                 ; preds = %foreach.head.L46
  %72 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %8), !dbg !95
  %73 = load i32, ptr %72, align 4, !dbg !95
  store i32 %73, ptr %item, align 4, !dbg !95
  %74 = load i32, ptr %item, align 4, !dbg !96
  %75 = add nsw i32 %74, 1, !dbg !96
  store i32 %75, ptr %item, align 4, !dbg !96
  br label %foreach.tail.L46, !dbg !97

foreach.tail.L46:                                 ; preds = %foreach.body.L46
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %8), !dbg !97
  br label %foreach.head.L46, !dbg !97

foreach.exit.L46:                                 ; preds = %foreach.head.L46
  %76 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !98
  %77 = load i32, ptr %76, align 4, !dbg !99
  %78 = icmp eq i32 %77, 123, !dbg !99
  br i1 %78, label %assert.exit.L49, label %assert.then.L49, !dbg !99, !prof !41

assert.then.L49:                                  ; preds = %foreach.exit.L46
  %79 = call i32 (ptr, ...) @printf(ptr @anon.string.16), !dbg !99
  call void @exit(i32 1), !dbg !99
  unreachable, !dbg !99

assert.exit.L49:                                  ; preds = %foreach.exit.L46
  %80 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !100
  %81 = load i32, ptr %80, align 4, !dbg !101
  %82 = icmp eq i32 %81, 4321, !dbg !101
  br i1 %82, label %assert.exit.L50, label %assert.then.L50, !dbg !101, !prof !41

assert.then.L50:                                  ; preds = %assert.exit.L49
  %83 = call i32 (ptr, ...) @printf(ptr @anon.string.17), !dbg !101
  call void @exit(i32 1), !dbg !101
  unreachable, !dbg !101

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %84 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !102
  %85 = load i32, ptr %84, align 4, !dbg !103
  %86 = icmp eq i32 %85, 9876, !dbg !103
  br i1 %86, label %assert.exit.L51, label %assert.then.L51, !dbg !103, !prof !41

assert.then.L51:                                  ; preds = %assert.exit.L50
  %87 = call i32 (ptr, ...) @printf(ptr @anon.string.18), !dbg !103
  call void @exit(i32 1), !dbg !103
  unreachable, !dbg !103

assert.exit.L51:                                  ; preds = %assert.exit.L50
  %88 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !104
  store %struct.VectorIterator %88, ptr %9, align 8, !dbg !104
    #dbg_declare(ptr %item1, !106, !DIExpression(), !107)
  br label %foreach.head.L54, !dbg !107

foreach.head.L54:                                 ; preds = %foreach.tail.L54, %assert.exit.L51
  %89 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %9), !dbg !107
  br i1 %89, label %foreach.body.L54, label %foreach.exit.L54, !dbg !107

foreach.body.L54:                                 ; preds = %foreach.head.L54
  %90 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %9), !dbg !107
  store ptr %90, ptr %10, align 8, !dbg !107
  %91 = load ptr, ptr %10, align 8, !dbg !108
  %92 = load i32, ptr %91, align 4, !dbg !108
  %93 = add nsw i32 %92, 1, !dbg !108
  store i32 %93, ptr %91, align 4, !dbg !108
  br label %foreach.tail.L54, !dbg !109

foreach.tail.L54:                                 ; preds = %foreach.body.L54
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %9), !dbg !109
  br label %foreach.head.L54, !dbg !109

foreach.exit.L54:                                 ; preds = %foreach.head.L54
  %94 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !110
  %95 = load i32, ptr %94, align 4, !dbg !111
  %96 = icmp eq i32 %95, 124, !dbg !111
  br i1 %96, label %assert.exit.L57, label %assert.then.L57, !dbg !111, !prof !41

assert.then.L57:                                  ; preds = %foreach.exit.L54
  %97 = call i32 (ptr, ...) @printf(ptr @anon.string.19), !dbg !111
  call void @exit(i32 1), !dbg !111
  unreachable, !dbg !111

assert.exit.L57:                                  ; preds = %foreach.exit.L54
  %98 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !112
  %99 = load i32, ptr %98, align 4, !dbg !113
  %100 = icmp eq i32 %99, 4322, !dbg !113
  br i1 %100, label %assert.exit.L58, label %assert.then.L58, !dbg !113, !prof !41

assert.then.L58:                                  ; preds = %assert.exit.L57
  %101 = call i32 (ptr, ...) @printf(ptr @anon.string.20), !dbg !113
  call void @exit(i32 1), !dbg !113
  unreachable, !dbg !113

assert.exit.L58:                                  ; preds = %assert.exit.L57
  %102 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !114
  %103 = load i32, ptr %102, align 4, !dbg !115
  %104 = icmp eq i32 %103, 9877, !dbg !115
  br i1 %104, label %assert.exit.L59, label %assert.then.L59, !dbg !115, !prof !41

assert.then.L59:                                  ; preds = %assert.exit.L58
  %105 = call i32 (ptr, ...) @printf(ptr @anon.string.21), !dbg !115
  call void @exit(i32 1), !dbg !115
  unreachable, !dbg !115

assert.exit.L59:                                  ; preds = %assert.exit.L58
  %106 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !116
  store %struct.VectorIterator %106, ptr %11, align 8, !dbg !116
  store i64 0, ptr %idx, align 8, !dbg !118
    #dbg_declare(ptr %idx, !119, !DIExpression(), !118)
    #dbg_declare(ptr %item2, !121, !DIExpression(), !122)
  br label %foreach.head.L61, !dbg !122

foreach.head.L61:                                 ; preds = %foreach.tail.L61, %assert.exit.L59
  %107 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %11), !dbg !122
  br i1 %107, label %foreach.body.L61, label %foreach.exit.L61, !dbg !122

foreach.body.L61:                                 ; preds = %foreach.head.L61
  %pair3 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr %11), !dbg !122
  store %struct.Pair %pair3, ptr %pair.addr, align 8, !dbg !122
  %108 = load i64, ptr %pair.addr, align 8, !dbg !122
  store i64 %108, ptr %idx, align 8, !dbg !122
  %item.addr = getelementptr inbounds nuw %struct.Pair, ptr %pair.addr, i32 0, i32 1, !dbg !122
  %109 = load ptr, ptr %item.addr, align 8, !dbg !122
  store ptr %109, ptr %12, align 8, !dbg !122
  %110 = load i64, ptr %idx, align 8, !dbg !123
  %111 = trunc i64 %110 to i32, !dbg !123
  %112 = load ptr, ptr %12, align 8, !dbg !123
  %113 = load i32, ptr %112, align 4, !dbg !123
  %114 = add nsw i32 %113, %111, !dbg !123
  store i32 %114, ptr %112, align 4, !dbg !123
  br label %foreach.tail.L61, !dbg !124

foreach.tail.L61:                                 ; preds = %foreach.body.L61
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %11), !dbg !124
  br label %foreach.head.L61, !dbg !124

foreach.exit.L61:                                 ; preds = %foreach.head.L61
  %115 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !125
  %116 = load i32, ptr %115, align 4, !dbg !126
  %117 = icmp eq i32 %116, 124, !dbg !126
  br i1 %117, label %assert.exit.L64, label %assert.then.L64, !dbg !126, !prof !41

assert.then.L64:                                  ; preds = %foreach.exit.L61
  %118 = call i32 (ptr, ...) @printf(ptr @anon.string.22), !dbg !126
  call void @exit(i32 1), !dbg !126
  unreachable, !dbg !126

assert.exit.L64:                                  ; preds = %foreach.exit.L61
  %119 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !127
  %120 = load i32, ptr %119, align 4, !dbg !128
  %121 = icmp eq i32 %120, 4323, !dbg !128
  br i1 %121, label %assert.exit.L65, label %assert.then.L65, !dbg !128, !prof !41

assert.then.L65:                                  ; preds = %assert.exit.L64
  %122 = call i32 (ptr, ...) @printf(ptr @anon.string.23), !dbg !128
  call void @exit(i32 1), !dbg !128
  unreachable, !dbg !128

assert.exit.L65:                                  ; preds = %assert.exit.L64
  %123 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !129
  %124 = load i32, ptr %123, align 4, !dbg !130
  %125 = icmp eq i32 %124, 9879, !dbg !130
  br i1 %125, label %assert.exit.L66, label %assert.then.L66, !dbg !130, !prof !41

assert.then.L66:                                  ; preds = %assert.exit.L65
  %126 = call i32 (ptr, ...) @printf(ptr @anon.string.24), !dbg !130
  call void @exit(i32 1), !dbg !130
  unreachable, !dbg !130

assert.exit.L66:                                  ; preds = %assert.exit.L65
  %127 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !131
  call void @_ZN6VectorIiE4dtorEv(ptr %vi), !dbg !132
  %128 = load i32, ptr %result, align 4, !dbg !132
  ret i32 %128, !dbg !132
}

declare void @_ZN6VectorIiE4ctorEv(ptr)

declare void @_ZN6VectorIiE8pushBackERKi(ptr, ptr)

declare i64 @_ZN6VectorIiE7getSizeEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) #1

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

declare void @_ZN6VectorIiE4dtorEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }

!llvm.module.flags = !{!7, !8, !9, !10, !11, !12}
!llvm.ident = !{!13}
!llvm.dbg.cu = !{!2}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !5, line: 68, type: !6, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/debug-info/success-dbg-info-complex/source.spice", directory: "./test-files/irgenerator/debug-info/success-dbg-info-complex")
!4 = !{!0}
!5 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/debug-info/success-dbg-info-complex")
!6 = !DIStringType(name: "printf.str.0", size: 192)
!7 = !{i32 8, !"PIC Level", i32 2}
!8 = !{i32 7, !"PIE Level", i32 2}
!9 = !{i32 7, !"uwtable", i32 2}
!10 = !{i32 7, !"frame-pointer", i32 2}
!11 = !{i32 7, !"Dwarf Version", i32 5}
!12 = !{i32 2, !"Debug Info Version", i32 3}
!13 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!14 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainiPPKc", scope: !5, file: !5, line: 4, type: !15, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !21)
!15 = !DISubroutineType(types: !16)
!16 = !{!17, !17, !18}
!17 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!18 = !DICompositeType(tag: DW_TAG_array_type, baseType: !19, elements: !21)
!19 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !20, size: 64)
!20 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!21 = !{}
!22 = !DILocalVariable(name: "result", scope: !14, file: !5, line: 4, type: !17)
!23 = !DILocation(line: 4, column: 1, scope: !14)
!24 = !DILocalVariable(name: "_argc", arg: 1, scope: !14, file: !5, line: 4, type: !17)
!25 = !DILocalVariable(name: "_argv", arg: 2, scope: !14, file: !5, line: 4, type: !18)
!26 = !DILocation(line: 6, column: 22, scope: !14)
!27 = !DILocalVariable(name: "vi", scope: !14, file: !5, line: 6, type: !28)
!28 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !5, file: !5, line: 25, size: 256, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !29, identifier: "struct.Vector")
!29 = !{!30, !32, !34}
!30 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !28, file: !5, line: 26, baseType: !31, size: 64, offset: 64)
!31 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !17, size: 64)
!32 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !28, file: !5, line: 27, baseType: !33, size: 64, offset: 128)
!33 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !28, file: !5, line: 28, baseType: !33, size: 64, offset: 192)
!35 = !DILocation(line: 6, column: 5, scope: !14)
!36 = !DILocation(line: 7, column: 17, scope: !14)
!37 = !DILocation(line: 8, column: 17, scope: !14)
!38 = !DILocation(line: 9, column: 17, scope: !14)
!39 = !DILocation(line: 10, column: 12, scope: !14)
!40 = !DILocation(line: 10, column: 28, scope: !14)
!41 = !{!"branch_weights", i32 2000, i32 1}
!42 = !DILocation(line: 13, column: 14, scope: !14)
!43 = !DILocalVariable(name: "it", scope: !14, file: !5, line: 13, type: !44)
!44 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator", scope: !5, file: !5, line: 280, size: 192, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !45, identifier: "struct.VectorIterator")
!45 = !{!46, !48}
!46 = !DIDerivedType(tag: DW_TAG_member, name: "vector", scope: !44, file: !5, line: 281, baseType: !47, size: 64, offset: 64)
!47 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !28, size: 64)
!48 = !DIDerivedType(tag: DW_TAG_member, name: "cursor", scope: !44, file: !5, line: 282, baseType: !33, size: 64, offset: 128)
!49 = !DILocation(line: 13, column: 5, scope: !14)
!50 = !DILocation(line: 14, column: 12, scope: !14)
!51 = !DILocation(line: 15, column: 12, scope: !14)
!52 = !DILocation(line: 15, column: 24, scope: !14)
!53 = !DILocation(line: 16, column: 12, scope: !14)
!54 = !DILocation(line: 16, column: 24, scope: !14)
!55 = !DILocation(line: 17, column: 5, scope: !14)
!56 = !DILocation(line: 18, column: 12, scope: !14)
!57 = !DILocation(line: 18, column: 24, scope: !14)
!58 = !DILocation(line: 19, column: 12, scope: !14)
!59 = !DILocation(line: 20, column: 5, scope: !14)
!60 = !DILocation(line: 21, column: 16, scope: !14)
!61 = !DILocalVariable(name: "pair", scope: !14, file: !5, line: 21, type: !62)
!62 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair", scope: !5, file: !5, line: 8, size: 128, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !63, identifier: "struct.Pair")
!63 = !{!64, !65}
!64 = !DIDerivedType(tag: DW_TAG_member, name: "first", scope: !62, file: !5, line: 9, baseType: !33, size: 64)
!65 = !DIDerivedType(tag: DW_TAG_member, name: "second", scope: !62, file: !5, line: 10, baseType: !66, size: 64, offset: 64)
!66 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !17, size: 64)
!67 = !DILocation(line: 21, column: 5, scope: !14)
!68 = !DILocation(line: 22, column: 12, scope: !14)
!69 = !DILocation(line: 22, column: 31, scope: !14)
!70 = !DILocation(line: 23, column: 12, scope: !14)
!71 = !DILocation(line: 23, column: 32, scope: !14)
!72 = !DILocation(line: 24, column: 5, scope: !14)
!73 = !DILocation(line: 25, column: 13, scope: !14)
!74 = !DILocation(line: 28, column: 17, scope: !14)
!75 = !DILocation(line: 29, column: 17, scope: !14)
!76 = !DILocation(line: 30, column: 12, scope: !14)
!77 = !DILocation(line: 33, column: 5, scope: !14)
!78 = !DILocation(line: 34, column: 12, scope: !14)
!79 = !DILocation(line: 34, column: 24, scope: !14)
!80 = !DILocation(line: 35, column: 12, scope: !14)
!81 = !DILocation(line: 36, column: 5, scope: !14)
!82 = !DILocation(line: 37, column: 12, scope: !14)
!83 = !DILocation(line: 37, column: 24, scope: !14)
!84 = !DILocation(line: 38, column: 5, scope: !14)
!85 = !DILocation(line: 39, column: 12, scope: !14)
!86 = !DILocation(line: 39, column: 24, scope: !14)
!87 = !DILocation(line: 40, column: 5, scope: !14)
!88 = !DILocation(line: 41, column: 12, scope: !14)
!89 = !DILocation(line: 41, column: 24, scope: !14)
!90 = !DILocation(line: 42, column: 5, scope: !14)
!91 = !DILocation(line: 43, column: 13, scope: !14)
!92 = !DILocation(line: 46, column: 24, scope: !93)
!93 = distinct !DILexicalBlock(scope: !14, file: !5, line: 46, column: 5)
!94 = !DILocalVariable(name: "item", scope: !93, file: !5, line: 46, type: !17)
!95 = !DILocation(line: 46, column: 13, scope: !93)
!96 = !DILocation(line: 47, column: 9, scope: !93)
!97 = !DILocation(line: 48, column: 5, scope: !93)
!98 = !DILocation(line: 49, column: 19, scope: !14)
!99 = !DILocation(line: 49, column: 25, scope: !14)
!100 = !DILocation(line: 50, column: 19, scope: !14)
!101 = !DILocation(line: 50, column: 25, scope: !14)
!102 = !DILocation(line: 51, column: 19, scope: !14)
!103 = !DILocation(line: 51, column: 25, scope: !14)
!104 = !DILocation(line: 54, column: 25, scope: !105)
!105 = distinct !DILexicalBlock(scope: !14, file: !5, line: 54, column: 5)
!106 = !DILocalVariable(name: "item", scope: !105, file: !5, line: 54, type: !66)
!107 = !DILocation(line: 54, column: 13, scope: !105)
!108 = !DILocation(line: 55, column: 9, scope: !105)
!109 = !DILocation(line: 56, column: 5, scope: !105)
!110 = !DILocation(line: 57, column: 19, scope: !14)
!111 = !DILocation(line: 57, column: 25, scope: !14)
!112 = !DILocation(line: 58, column: 19, scope: !14)
!113 = !DILocation(line: 58, column: 25, scope: !14)
!114 = !DILocation(line: 59, column: 19, scope: !14)
!115 = !DILocation(line: 59, column: 25, scope: !14)
!116 = !DILocation(line: 61, column: 35, scope: !117)
!117 = distinct !DILexicalBlock(scope: !14, file: !5, line: 61, column: 5)
!118 = !DILocation(line: 61, column: 13, scope: !117)
!119 = !DILocalVariable(name: "idx", scope: !117, file: !5, line: 61, type: !120)
!120 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!121 = !DILocalVariable(name: "item", scope: !117, file: !5, line: 61, type: !66)
!122 = !DILocation(line: 61, column: 23, scope: !117)
!123 = !DILocation(line: 62, column: 9, scope: !117)
!124 = !DILocation(line: 63, column: 5, scope: !117)
!125 = !DILocation(line: 64, column: 19, scope: !14)
!126 = !DILocation(line: 64, column: 25, scope: !14)
!127 = !DILocation(line: 65, column: 19, scope: !14)
!128 = !DILocation(line: 65, column: 25, scope: !14)
!129 = !DILocation(line: 66, column: 19, scope: !14)
!130 = !DILocation(line: 66, column: 25, scope: !14)
!131 = !DILocation(line: 68, column: 5, scope: !14)
!132 = !DILocation(line: 69, column: 1, scope: !14)
