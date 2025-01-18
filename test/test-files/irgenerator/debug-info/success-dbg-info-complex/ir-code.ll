; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Vector = type { %interface.IIterable, ptr, i64, i64 }
%interface.IIterable = type { ptr }
%struct.VectorIterator = type { %interface.IIterator, ptr, i64 }
%interface.IIterator = type { ptr }
%struct.Pair = type { i64, ptr }

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.getSize() == 3' evaluated to false.\00", align 1
@anon.string.1 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.3 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.4 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'pair.getFirst() == 2' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 'pair.getSecond() == 9876' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == -99' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\00", align 1
@anon.string.16 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 123' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4321' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9876' evaluated to false.\00", align 1
@anon.string.19 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 124' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4322' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9877' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 124' evaluated to false.\00", align 1
@anon.string.23 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4323' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9879' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 1, !dbg !0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) #0 !dbg !15 {
    #dbg_declare(ptr %result, !23, !DIExpression(), !24)
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
  %pair_addr = alloca %struct.Pair, align 8
  %12 = alloca ptr, align 8
  store i32 0, ptr %result, align 4, !dbg !24
    #dbg_declare(ptr %_argc, !25, !DIExpression(), !24)
  store i32 %0, ptr %_argc, align 4, !dbg !24
    #dbg_declare(ptr %_argv, !26, !DIExpression(), !24)
  store ptr %1, ptr %_argv, align 8, !dbg !24
    #dbg_declare(ptr %vi, !27, !DIExpression(), !35)
  call void @_ZN6VectorIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !36
  store i32 123, ptr %3, align 4, !dbg !37
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %3), !dbg !37
  store i32 4321, ptr %4, align 4, !dbg !38
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %4), !dbg !38
  store i32 9876, ptr %5, align 4, !dbg !39
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %5), !dbg !39
  %13 = call i64 @_ZN6VectorIiE7getSizeEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !40
  %14 = icmp eq i64 %13, 3, !dbg !41
  br i1 %14, label %assert.exit.L10, label %assert.then.L10, !dbg !41, !prof !42

assert.then.L10:                                  ; preds = %2
  %15 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !41
  call void @exit(i32 1), !dbg !41
  unreachable, !dbg !41

assert.exit.L10:                                  ; preds = %2
  %16 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !43
    #dbg_declare(ptr %it, !44, !DIExpression(), !50)
  store %struct.VectorIterator %16, ptr %it, align 8, !dbg !43
  %17 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !51
  br i1 %17, label %assert.exit.L14, label %assert.then.L14, !dbg !51, !prof !42

assert.then.L14:                                  ; preds = %assert.exit.L10
  %18 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !51
  call void @exit(i32 1), !dbg !51
  unreachable, !dbg !51

assert.exit.L14:                                  ; preds = %assert.exit.L10
  %19 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !52
  %20 = load i32, ptr %19, align 4, !dbg !53
  %21 = icmp eq i32 %20, 123, !dbg !53
  br i1 %21, label %assert.exit.L15, label %assert.then.L15, !dbg !53, !prof !42

assert.then.L15:                                  ; preds = %assert.exit.L14
  %22 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !53
  call void @exit(i32 1), !dbg !53
  unreachable, !dbg !53

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %23 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !54
  %24 = load i32, ptr %23, align 4, !dbg !55
  %25 = icmp eq i32 %24, 123, !dbg !55
  br i1 %25, label %assert.exit.L16, label %assert.then.L16, !dbg !55, !prof !42

assert.then.L16:                                  ; preds = %assert.exit.L15
  %26 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !55
  call void @exit(i32 1), !dbg !55
  unreachable, !dbg !55

assert.exit.L16:                                  ; preds = %assert.exit.L15
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !56
  %27 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !57
  %28 = load i32, ptr %27, align 4, !dbg !58
  %29 = icmp eq i32 %28, 4321, !dbg !58
  br i1 %29, label %assert.exit.L18, label %assert.then.L18, !dbg !58, !prof !42

assert.then.L18:                                  ; preds = %assert.exit.L16
  %30 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !58
  call void @exit(i32 1), !dbg !58
  unreachable, !dbg !58

assert.exit.L18:                                  ; preds = %assert.exit.L16
  %31 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !59
  br i1 %31, label %assert.exit.L19, label %assert.then.L19, !dbg !59, !prof !42

assert.then.L19:                                  ; preds = %assert.exit.L18
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !59
  call void @exit(i32 1), !dbg !59
  unreachable, !dbg !59

assert.exit.L19:                                  ; preds = %assert.exit.L18
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !60
  %33 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !61
    #dbg_declare(ptr %pair, !62, !DIExpression(), !68)
  store %struct.Pair %33, ptr %pair, align 8, !dbg !61
  %34 = call ptr @_ZN4PairImRiE8getFirstEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !69
  %35 = load i64, ptr %34, align 8, !dbg !70
  %36 = icmp eq i64 %35, 2, !dbg !70
  br i1 %36, label %assert.exit.L22, label %assert.then.L22, !dbg !70, !prof !42

assert.then.L22:                                  ; preds = %assert.exit.L19
  %37 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !70
  call void @exit(i32 1), !dbg !70
  unreachable, !dbg !70

assert.exit.L22:                                  ; preds = %assert.exit.L19
  %38 = call ptr @_ZN4PairImRiE9getSecondEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !71
  %39 = load i32, ptr %38, align 4, !dbg !72
  %40 = icmp eq i32 %39, 9876, !dbg !72
  br i1 %40, label %assert.exit.L23, label %assert.then.L23, !dbg !72, !prof !42

assert.then.L23:                                  ; preds = %assert.exit.L22
  %41 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !72
  call void @exit(i32 1), !dbg !72
  unreachable, !dbg !72

assert.exit.L23:                                  ; preds = %assert.exit.L22
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !73
  %42 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !74
  %43 = xor i1 %42, true, !dbg !74
  br i1 %43, label %assert.exit.L25, label %assert.then.L25, !dbg !74, !prof !42

assert.then.L25:                                  ; preds = %assert.exit.L23
  %44 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !74
  call void @exit(i32 1), !dbg !74
  unreachable, !dbg !74

assert.exit.L25:                                  ; preds = %assert.exit.L23
  store i32 321, ptr %6, align 4, !dbg !75
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %6), !dbg !75
  store i32 -99, ptr %7, align 4, !dbg !76
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %7), !dbg !76
  %45 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !77
  br i1 %45, label %assert.exit.L30, label %assert.then.L30, !dbg !77, !prof !42

assert.then.L30:                                  ; preds = %assert.exit.L25
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !77
  call void @exit(i32 1), !dbg !77
  unreachable, !dbg !77

assert.exit.L30:                                  ; preds = %assert.exit.L25
  call void @_Z13op.minusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 3), !dbg !78
  %47 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !79
  %48 = load i32, ptr %47, align 4, !dbg !80
  %49 = icmp eq i32 %48, 123, !dbg !80
  br i1 %49, label %assert.exit.L34, label %assert.then.L34, !dbg !80, !prof !42

assert.then.L34:                                  ; preds = %assert.exit.L30
  %50 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !80
  call void @exit(i32 1), !dbg !80
  unreachable, !dbg !80

assert.exit.L34:                                  ; preds = %assert.exit.L30
  %51 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !81
  br i1 %51, label %assert.exit.L35, label %assert.then.L35, !dbg !81, !prof !42

assert.then.L35:                                  ; preds = %assert.exit.L34
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !81
  call void @exit(i32 1), !dbg !81
  unreachable, !dbg !81

assert.exit.L35:                                  ; preds = %assert.exit.L34
  %53 = load %struct.VectorIterator, ptr %it, align 8, !dbg !82
  call void @_Z16op.plusplus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !82
  %54 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !83
  %55 = load i32, ptr %54, align 4, !dbg !84
  %56 = icmp eq i32 %55, 4321, !dbg !84
  br i1 %56, label %assert.exit.L37, label %assert.then.L37, !dbg !84, !prof !42

assert.then.L37:                                  ; preds = %assert.exit.L35
  %57 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !84
  call void @exit(i32 1), !dbg !84
  unreachable, !dbg !84

assert.exit.L37:                                  ; preds = %assert.exit.L35
  %58 = load %struct.VectorIterator, ptr %it, align 8, !dbg !85
  call void @_Z18op.minusminus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !85
  %59 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !86
  %60 = load i32, ptr %59, align 4, !dbg !87
  %61 = icmp eq i32 %60, 123, !dbg !87
  br i1 %61, label %assert.exit.L39, label %assert.then.L39, !dbg !87, !prof !42

assert.then.L39:                                  ; preds = %assert.exit.L37
  %62 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !87
  call void @exit(i32 1), !dbg !87
  unreachable, !dbg !87

assert.exit.L39:                                  ; preds = %assert.exit.L37
  call void @_Z12op.plusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 4), !dbg !88
  %63 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !89
  %64 = load i32, ptr %63, align 4, !dbg !90
  %65 = icmp eq i32 %64, -99, !dbg !90
  br i1 %65, label %assert.exit.L41, label %assert.then.L41, !dbg !90, !prof !42

assert.then.L41:                                  ; preds = %assert.exit.L39
  %66 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !90
  call void @exit(i32 1), !dbg !90
  unreachable, !dbg !90

assert.exit.L41:                                  ; preds = %assert.exit.L39
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !91
  %67 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !92
  %68 = xor i1 %67, true, !dbg !92
  br i1 %68, label %assert.exit.L43, label %assert.then.L43, !dbg !92, !prof !42

assert.then.L43:                                  ; preds = %assert.exit.L41
  %69 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !92
  call void @exit(i32 1), !dbg !92
  unreachable, !dbg !92

assert.exit.L43:                                  ; preds = %assert.exit.L41
  %70 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !93
    #dbg_declare(ptr %item, !95, !DIExpression(), !96)
  store %struct.VectorIterator %70, ptr %8, align 8, !dbg !93
  br label %foreach.head.L46, !dbg !96

foreach.head.L46:                                 ; preds = %foreach.tail.L46, %assert.exit.L43
  %71 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %8), !dbg !96
  br i1 %71, label %foreach.body.L46, label %foreach.exit.L46, !dbg !96

foreach.body.L46:                                 ; preds = %foreach.head.L46
  %72 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %8), !dbg !96
  %73 = load i32, ptr %72, align 4, !dbg !96
  store i32 %73, ptr %item, align 4, !dbg !96
  %74 = load i32, ptr %item, align 4, !dbg !97
  %75 = add nsw i32 %74, 1, !dbg !97
  store i32 %75, ptr %item, align 4, !dbg !97
  br label %foreach.tail.L46, !dbg !98

foreach.tail.L46:                                 ; preds = %foreach.body.L46
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %8), !dbg !98
  br label %foreach.head.L46, !dbg !98

foreach.exit.L46:                                 ; preds = %foreach.head.L46
  %76 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !99
  %77 = load i32, ptr %76, align 4, !dbg !100
  %78 = icmp eq i32 %77, 123, !dbg !100
  br i1 %78, label %assert.exit.L49, label %assert.then.L49, !dbg !100, !prof !42

assert.then.L49:                                  ; preds = %foreach.exit.L46
  %79 = call i32 (ptr, ...) @printf(ptr @anon.string.16), !dbg !100
  call void @exit(i32 1), !dbg !100
  unreachable, !dbg !100

assert.exit.L49:                                  ; preds = %foreach.exit.L46
  %80 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !101
  %81 = load i32, ptr %80, align 4, !dbg !102
  %82 = icmp eq i32 %81, 4321, !dbg !102
  br i1 %82, label %assert.exit.L50, label %assert.then.L50, !dbg !102, !prof !42

assert.then.L50:                                  ; preds = %assert.exit.L49
  %83 = call i32 (ptr, ...) @printf(ptr @anon.string.17), !dbg !102
  call void @exit(i32 1), !dbg !102
  unreachable, !dbg !102

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %84 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !103
  %85 = load i32, ptr %84, align 4, !dbg !104
  %86 = icmp eq i32 %85, 9876, !dbg !104
  br i1 %86, label %assert.exit.L51, label %assert.then.L51, !dbg !104, !prof !42

assert.then.L51:                                  ; preds = %assert.exit.L50
  %87 = call i32 (ptr, ...) @printf(ptr @anon.string.18), !dbg !104
  call void @exit(i32 1), !dbg !104
  unreachable, !dbg !104

assert.exit.L51:                                  ; preds = %assert.exit.L50
  %88 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !105
    #dbg_declare(ptr %item1, !107, !DIExpression(), !108)
  store %struct.VectorIterator %88, ptr %9, align 8, !dbg !105
  br label %foreach.head.L54, !dbg !108

foreach.head.L54:                                 ; preds = %foreach.tail.L54, %assert.exit.L51
  %89 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %9), !dbg !108
  br i1 %89, label %foreach.body.L54, label %foreach.exit.L54, !dbg !108

foreach.body.L54:                                 ; preds = %foreach.head.L54
  %90 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %9), !dbg !108
  store ptr %90, ptr %10, align 8, !dbg !108
  %91 = load ptr, ptr %10, align 8, !dbg !109
  %92 = load i32, ptr %91, align 4, !dbg !109
  %93 = add nsw i32 %92, 1, !dbg !109
  store i32 %93, ptr %91, align 4, !dbg !109
  br label %foreach.tail.L54, !dbg !110

foreach.tail.L54:                                 ; preds = %foreach.body.L54
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %9), !dbg !110
  br label %foreach.head.L54, !dbg !110

foreach.exit.L54:                                 ; preds = %foreach.head.L54
  %94 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !111
  %95 = load i32, ptr %94, align 4, !dbg !112
  %96 = icmp eq i32 %95, 124, !dbg !112
  br i1 %96, label %assert.exit.L57, label %assert.then.L57, !dbg !112, !prof !42

assert.then.L57:                                  ; preds = %foreach.exit.L54
  %97 = call i32 (ptr, ...) @printf(ptr @anon.string.19), !dbg !112
  call void @exit(i32 1), !dbg !112
  unreachable, !dbg !112

assert.exit.L57:                                  ; preds = %foreach.exit.L54
  %98 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !113
  %99 = load i32, ptr %98, align 4, !dbg !114
  %100 = icmp eq i32 %99, 4322, !dbg !114
  br i1 %100, label %assert.exit.L58, label %assert.then.L58, !dbg !114, !prof !42

assert.then.L58:                                  ; preds = %assert.exit.L57
  %101 = call i32 (ptr, ...) @printf(ptr @anon.string.20), !dbg !114
  call void @exit(i32 1), !dbg !114
  unreachable, !dbg !114

assert.exit.L58:                                  ; preds = %assert.exit.L57
  %102 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !115
  %103 = load i32, ptr %102, align 4, !dbg !116
  %104 = icmp eq i32 %103, 9877, !dbg !116
  br i1 %104, label %assert.exit.L59, label %assert.then.L59, !dbg !116, !prof !42

assert.then.L59:                                  ; preds = %assert.exit.L58
  %105 = call i32 (ptr, ...) @printf(ptr @anon.string.21), !dbg !116
  call void @exit(i32 1), !dbg !116
  unreachable, !dbg !116

assert.exit.L59:                                  ; preds = %assert.exit.L58
  %106 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !117
  store %struct.VectorIterator %106, ptr %11, align 8, !dbg !117
    #dbg_declare(ptr %idx, !119, !DIExpression(), !121)
    #dbg_declare(ptr %item2, !122, !DIExpression(), !123)
  store i64 0, ptr %idx, align 8, !dbg !121
  br label %foreach.head.L61, !dbg !123

foreach.head.L61:                                 ; preds = %foreach.tail.L61, %assert.exit.L59
  %107 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %11), !dbg !123
  br i1 %107, label %foreach.body.L61, label %foreach.exit.L61, !dbg !123

foreach.body.L61:                                 ; preds = %foreach.head.L61
  %pair3 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr %11), !dbg !123
  store %struct.Pair %pair3, ptr %pair_addr, align 8, !dbg !123
  %108 = load i64, ptr %pair_addr, align 8, !dbg !123
  store i64 %108, ptr %idx, align 8, !dbg !123
  %item_addr = getelementptr inbounds %struct.Pair, ptr %pair_addr, i32 0, i32 1, !dbg !123
  %109 = load ptr, ptr %item_addr, align 8, !dbg !123
  store ptr %109, ptr %12, align 8, !dbg !123
  %110 = load i64, ptr %idx, align 8, !dbg !124
  %111 = trunc i64 %110 to i32, !dbg !124
  %112 = load ptr, ptr %12, align 8, !dbg !124
  %113 = load i32, ptr %112, align 4, !dbg !124
  %114 = add nsw i32 %113, %111, !dbg !124
  store i32 %114, ptr %112, align 4, !dbg !124
  br label %foreach.tail.L61, !dbg !125

foreach.tail.L61:                                 ; preds = %foreach.body.L61
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %11), !dbg !125
  br label %foreach.head.L61, !dbg !125

foreach.exit.L61:                                 ; preds = %foreach.head.L61
  %115 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !126
  %116 = load i32, ptr %115, align 4, !dbg !127
  %117 = icmp eq i32 %116, 124, !dbg !127
  br i1 %117, label %assert.exit.L64, label %assert.then.L64, !dbg !127, !prof !42

assert.then.L64:                                  ; preds = %foreach.exit.L61
  %118 = call i32 (ptr, ...) @printf(ptr @anon.string.22), !dbg !127
  call void @exit(i32 1), !dbg !127
  unreachable, !dbg !127

assert.exit.L64:                                  ; preds = %foreach.exit.L61
  %119 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !128
  %120 = load i32, ptr %119, align 4, !dbg !129
  %121 = icmp eq i32 %120, 4323, !dbg !129
  br i1 %121, label %assert.exit.L65, label %assert.then.L65, !dbg !129, !prof !42

assert.then.L65:                                  ; preds = %assert.exit.L64
  %122 = call i32 (ptr, ...) @printf(ptr @anon.string.23), !dbg !129
  call void @exit(i32 1), !dbg !129
  unreachable, !dbg !129

assert.exit.L65:                                  ; preds = %assert.exit.L64
  %123 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !130
  %124 = load i32, ptr %123, align 4, !dbg !131
  %125 = icmp eq i32 %124, 9879, !dbg !131
  br i1 %125, label %assert.exit.L66, label %assert.then.L66, !dbg !131, !prof !42

assert.then.L66:                                  ; preds = %assert.exit.L65
  %126 = call i32 (ptr, ...) @printf(ptr @anon.string.24), !dbg !131
  call void @exit(i32 1), !dbg !131
  unreachable, !dbg !131

assert.exit.L66:                                  ; preds = %assert.exit.L65
  %127 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !132
  call void @_ZN6VectorIiE4dtorEv(ptr %vi), !dbg !133
  %128 = load i32, ptr %result, align 4, !dbg !133
  ret i32 %128, !dbg !133
}

declare void @_ZN6VectorIiE4ctorEv(ptr)

declare void @_ZN6VectorIiE8pushBackERKi(ptr, ptr)

declare i64 @_ZN6VectorIiE7getSizeEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #1

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

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!7, !8, !9, !10, !11, !12, !13}
!llvm.ident = !{!14}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !5, line: 68, type: !6, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Documents/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/debug-info/success-dbg-info-complex/source.spice", directory: "./test-files/irgenerator/debug-info/success-dbg-info-complex")
!4 = !{!0}
!5 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/debug-info/success-dbg-info-complex")
!6 = !DIStringType(name: "printf.str.0", size: 192)
!7 = !{i32 7, !"Dwarf Version", i32 4}
!8 = !{i32 2, !"Debug Info Version", i32 3}
!9 = !{i32 1, !"wchar_size", i32 4}
!10 = !{i32 8, !"PIC Level", i32 2}
!11 = !{i32 7, !"PIE Level", i32 2}
!12 = !{i32 7, !"uwtable", i32 2}
!13 = !{i32 7, !"frame-pointer", i32 2}
!14 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!15 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainiPPKc", scope: !5, file: !5, line: 4, type: !16, scopeLine: 4, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !22)
!16 = !DISubroutineType(types: !17)
!17 = !{!18, !18, !19}
!18 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!19 = !DICompositeType(tag: DW_TAG_array_type, baseType: !20, elements: !22)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !21, size: 64)
!21 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!22 = !{}
!23 = !DILocalVariable(name: "result", scope: !15, file: !5, line: 4, type: !18)
!24 = !DILocation(line: 4, column: 1, scope: !15)
!25 = !DILocalVariable(name: "_argc", arg: 1, scope: !15, file: !5, line: 4, type: !18)
!26 = !DILocalVariable(name: "_argv", arg: 2, scope: !15, file: !5, line: 4, type: !19)
!27 = !DILocalVariable(name: "vi", scope: !15, file: !5, line: 6, type: !28)
!28 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !5, file: !5, line: 25, size: 256, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !29, identifier: "struct.Vector")
!29 = !{!30, !32, !34}
!30 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !28, file: !5, line: 26, baseType: !31, size: 64, offset: 64)
!31 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64)
!32 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !28, file: !5, line: 27, baseType: !33, size: 64, offset: 128)
!33 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!34 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !28, file: !5, line: 28, baseType: !33, size: 64, offset: 192)
!35 = !DILocation(line: 6, column: 5, scope: !15)
!36 = !DILocation(line: 6, column: 22, scope: !15)
!37 = !DILocation(line: 7, column: 17, scope: !15)
!38 = !DILocation(line: 8, column: 17, scope: !15)
!39 = !DILocation(line: 9, column: 17, scope: !15)
!40 = !DILocation(line: 10, column: 12, scope: !15)
!41 = !DILocation(line: 10, column: 28, scope: !15)
!42 = !{!"branch_weights", i32 2000, i32 1}
!43 = !DILocation(line: 13, column: 14, scope: !15)
!44 = !DILocalVariable(name: "it", scope: !15, file: !5, line: 13, type: !45)
!45 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator", scope: !5, file: !5, line: 280, size: 192, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !46, identifier: "struct.VectorIterator")
!46 = !{!47, !49}
!47 = !DIDerivedType(tag: DW_TAG_member, name: "vector", scope: !45, file: !5, line: 281, baseType: !48, size: 64, offset: 64)
!48 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !28, size: 64)
!49 = !DIDerivedType(tag: DW_TAG_member, name: "cursor", scope: !45, file: !5, line: 282, baseType: !33, size: 64, offset: 128)
!50 = !DILocation(line: 13, column: 5, scope: !15)
!51 = !DILocation(line: 14, column: 12, scope: !15)
!52 = !DILocation(line: 15, column: 12, scope: !15)
!53 = !DILocation(line: 15, column: 24, scope: !15)
!54 = !DILocation(line: 16, column: 12, scope: !15)
!55 = !DILocation(line: 16, column: 24, scope: !15)
!56 = !DILocation(line: 17, column: 5, scope: !15)
!57 = !DILocation(line: 18, column: 12, scope: !15)
!58 = !DILocation(line: 18, column: 24, scope: !15)
!59 = !DILocation(line: 19, column: 12, scope: !15)
!60 = !DILocation(line: 20, column: 5, scope: !15)
!61 = !DILocation(line: 21, column: 16, scope: !15)
!62 = !DILocalVariable(name: "pair", scope: !15, file: !5, line: 21, type: !63)
!63 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair", scope: !5, file: !5, line: 8, size: 128, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !64, identifier: "struct.Pair")
!64 = !{!65, !66}
!65 = !DIDerivedType(tag: DW_TAG_member, name: "first", scope: !63, file: !5, line: 9, baseType: !33, size: 64)
!66 = !DIDerivedType(tag: DW_TAG_member, name: "second", scope: !63, file: !5, line: 10, baseType: !67, size: 64, offset: 64)
!67 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !18, size: 64)
!68 = !DILocation(line: 21, column: 5, scope: !15)
!69 = !DILocation(line: 22, column: 12, scope: !15)
!70 = !DILocation(line: 22, column: 31, scope: !15)
!71 = !DILocation(line: 23, column: 12, scope: !15)
!72 = !DILocation(line: 23, column: 32, scope: !15)
!73 = !DILocation(line: 24, column: 5, scope: !15)
!74 = !DILocation(line: 25, column: 13, scope: !15)
!75 = !DILocation(line: 28, column: 17, scope: !15)
!76 = !DILocation(line: 29, column: 17, scope: !15)
!77 = !DILocation(line: 30, column: 12, scope: !15)
!78 = !DILocation(line: 33, column: 5, scope: !15)
!79 = !DILocation(line: 34, column: 12, scope: !15)
!80 = !DILocation(line: 34, column: 24, scope: !15)
!81 = !DILocation(line: 35, column: 12, scope: !15)
!82 = !DILocation(line: 36, column: 5, scope: !15)
!83 = !DILocation(line: 37, column: 12, scope: !15)
!84 = !DILocation(line: 37, column: 24, scope: !15)
!85 = !DILocation(line: 38, column: 5, scope: !15)
!86 = !DILocation(line: 39, column: 12, scope: !15)
!87 = !DILocation(line: 39, column: 24, scope: !15)
!88 = !DILocation(line: 40, column: 5, scope: !15)
!89 = !DILocation(line: 41, column: 12, scope: !15)
!90 = !DILocation(line: 41, column: 24, scope: !15)
!91 = !DILocation(line: 42, column: 5, scope: !15)
!92 = !DILocation(line: 43, column: 13, scope: !15)
!93 = !DILocation(line: 46, column: 24, scope: !94)
!94 = distinct !DILexicalBlock(scope: !15, file: !5, line: 46, column: 5)
!95 = !DILocalVariable(name: "item", scope: !94, file: !5, line: 46, type: !18)
!96 = !DILocation(line: 46, column: 13, scope: !94)
!97 = !DILocation(line: 47, column: 9, scope: !94)
!98 = !DILocation(line: 48, column: 5, scope: !94)
!99 = !DILocation(line: 49, column: 19, scope: !15)
!100 = !DILocation(line: 49, column: 25, scope: !15)
!101 = !DILocation(line: 50, column: 19, scope: !15)
!102 = !DILocation(line: 50, column: 25, scope: !15)
!103 = !DILocation(line: 51, column: 19, scope: !15)
!104 = !DILocation(line: 51, column: 25, scope: !15)
!105 = !DILocation(line: 54, column: 25, scope: !106)
!106 = distinct !DILexicalBlock(scope: !15, file: !5, line: 54, column: 5)
!107 = !DILocalVariable(name: "item", scope: !106, file: !5, line: 54, type: !67)
!108 = !DILocation(line: 54, column: 13, scope: !106)
!109 = !DILocation(line: 55, column: 9, scope: !106)
!110 = !DILocation(line: 56, column: 5, scope: !106)
!111 = !DILocation(line: 57, column: 19, scope: !15)
!112 = !DILocation(line: 57, column: 25, scope: !15)
!113 = !DILocation(line: 58, column: 19, scope: !15)
!114 = !DILocation(line: 58, column: 25, scope: !15)
!115 = !DILocation(line: 59, column: 19, scope: !15)
!116 = !DILocation(line: 59, column: 25, scope: !15)
!117 = !DILocation(line: 61, column: 35, scope: !118)
!118 = distinct !DILexicalBlock(scope: !15, file: !5, line: 61, column: 5)
!119 = !DILocalVariable(name: "idx", scope: !118, file: !5, line: 61, type: !120)
!120 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!121 = !DILocation(line: 61, column: 13, scope: !118)
!122 = !DILocalVariable(name: "item", scope: !118, file: !5, line: 61, type: !67)
!123 = !DILocation(line: 61, column: 23, scope: !118)
!124 = !DILocation(line: 62, column: 9, scope: !118)
!125 = !DILocation(line: 63, column: 5, scope: !118)
!126 = !DILocation(line: 64, column: 19, scope: !15)
!127 = !DILocation(line: 64, column: 25, scope: !15)
!128 = !DILocation(line: 65, column: 19, scope: !15)
!129 = !DILocation(line: 65, column: 25, scope: !15)
!130 = !DILocation(line: 66, column: 19, scope: !15)
!131 = !DILocation(line: 66, column: 25, scope: !15)
!132 = !DILocation(line: 68, column: 5, scope: !15)
!133 = !DILocation(line: 69, column: 1, scope: !15)
