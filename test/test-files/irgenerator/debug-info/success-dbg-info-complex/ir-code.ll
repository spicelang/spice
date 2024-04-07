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
  %result = alloca i32, align 4
  %_argc = alloca i32, align 4
  %_argv = alloca ptr, align 8
  %vi = alloca %struct.Vector, align 8
  %3 = alloca i32, align 4
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %it = alloca %struct.VectorIterator, align 8
  %pair = alloca %struct.Pair, align 8
  %6 = alloca i1, align 1
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i1, align 1
  %10 = alloca %struct.VectorIterator, align 8
  %item = alloca i32, align 4
  %11 = alloca %struct.VectorIterator, align 8
  %item1 = alloca ptr, align 8
  %12 = alloca ptr, align 8
  %13 = alloca %struct.VectorIterator, align 8
  %idx = alloca i64, align 8
  %item2 = alloca ptr, align 8
  %pair_addr = alloca %struct.Pair, align 8
  %14 = alloca ptr, align 8
  call void @llvm.dbg.declare(metadata ptr %result, metadata !23, metadata !DIExpression()), !dbg !24
  store i32 0, ptr %result, align 4, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %_argc, metadata !25, metadata !DIExpression()), !dbg !24
  store i32 %0, ptr %_argc, align 4, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %_argv, metadata !26, metadata !DIExpression()), !dbg !24
  store ptr %1, ptr %_argv, align 8, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %vi, metadata !27, metadata !DIExpression()), !dbg !36
  call void @_ZN6VectorIiE4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !37
  store i32 123, ptr %3, align 4, !dbg !38
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %3), !dbg !38
  store i32 4321, ptr %4, align 4, !dbg !39
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %4), !dbg !39
  store i32 9876, ptr %5, align 4, !dbg !40
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %5), !dbg !40
  %15 = call i64 @_ZN6VectorIiE7getSizeEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !41
  %16 = icmp eq i64 %15, 3, !dbg !42
  br i1 %16, label %assert.exit.L10, label %assert.then.L10, !dbg !42, !prof !43

assert.then.L10:                                  ; preds = %2
  %17 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !42
  call void @exit(i32 1), !dbg !42
  unreachable, !dbg !42

assert.exit.L10:                                  ; preds = %2
  %18 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !44
  call void @llvm.dbg.declare(metadata ptr %it, metadata !45, metadata !DIExpression()), !dbg !51
  store %struct.VectorIterator %18, ptr %it, align 8, !dbg !44
  %19 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !52
  br i1 %19, label %assert.exit.L14, label %assert.then.L14, !dbg !52, !prof !43

assert.then.L14:                                  ; preds = %assert.exit.L10
  %20 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !52
  call void @exit(i32 1), !dbg !52
  unreachable, !dbg !52

assert.exit.L14:                                  ; preds = %assert.exit.L10
  %21 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !53
  %22 = load i32, ptr %21, align 4, !dbg !54
  %23 = icmp eq i32 %22, 123, !dbg !54
  br i1 %23, label %assert.exit.L15, label %assert.then.L15, !dbg !54, !prof !43

assert.then.L15:                                  ; preds = %assert.exit.L14
  %24 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !54
  call void @exit(i32 1), !dbg !54
  unreachable, !dbg !54

assert.exit.L15:                                  ; preds = %assert.exit.L14
  %25 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !55
  %26 = load i32, ptr %25, align 4, !dbg !56
  %27 = icmp eq i32 %26, 123, !dbg !56
  br i1 %27, label %assert.exit.L16, label %assert.then.L16, !dbg !56, !prof !43

assert.then.L16:                                  ; preds = %assert.exit.L15
  %28 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !56
  call void @exit(i32 1), !dbg !56
  unreachable, !dbg !56

assert.exit.L16:                                  ; preds = %assert.exit.L15
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !57
  %29 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !58
  %30 = load i32, ptr %29, align 4, !dbg !59
  %31 = icmp eq i32 %30, 4321, !dbg !59
  br i1 %31, label %assert.exit.L18, label %assert.then.L18, !dbg !59, !prof !43

assert.then.L18:                                  ; preds = %assert.exit.L16
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !59
  call void @exit(i32 1), !dbg !59
  unreachable, !dbg !59

assert.exit.L18:                                  ; preds = %assert.exit.L16
  %33 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !60
  br i1 %33, label %assert.exit.L19, label %assert.then.L19, !dbg !60, !prof !43

assert.then.L19:                                  ; preds = %assert.exit.L18
  %34 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !60
  call void @exit(i32 1), !dbg !60
  unreachable, !dbg !60

assert.exit.L19:                                  ; preds = %assert.exit.L18
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !61
  %35 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !62
  call void @llvm.dbg.declare(metadata ptr %pair, metadata !63, metadata !DIExpression()), !dbg !70
  store %struct.Pair %35, ptr %pair, align 8, !dbg !62
  %36 = call ptr @_ZN4PairImRiE8getFirstEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !71
  %37 = load i64, ptr %36, align 8, !dbg !72
  %38 = icmp eq i64 %37, 2, !dbg !72
  br i1 %38, label %assert.exit.L22, label %assert.then.L22, !dbg !72, !prof !43

assert.then.L22:                                  ; preds = %assert.exit.L19
  %39 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !72
  call void @exit(i32 1), !dbg !72
  unreachable, !dbg !72

assert.exit.L22:                                  ; preds = %assert.exit.L19
  %40 = call ptr @_ZN4PairImRiE9getSecondEv(ptr noundef nonnull align 8 dereferenceable(16) %pair), !dbg !73
  %41 = load i32, ptr %40, align 4, !dbg !74
  %42 = icmp eq i32 %41, 9876, !dbg !74
  br i1 %42, label %assert.exit.L23, label %assert.then.L23, !dbg !74, !prof !43

assert.then.L23:                                  ; preds = %assert.exit.L22
  %43 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !74
  call void @exit(i32 1), !dbg !74
  unreachable, !dbg !74

assert.exit.L23:                                  ; preds = %assert.exit.L22
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !75
  %44 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !76
  %45 = xor i1 %44, true, !dbg !76
  store i1 %45, ptr %6, align 1, !dbg !76
  br i1 %45, label %assert.exit.L25, label %assert.then.L25, !dbg !76, !prof !43

assert.then.L25:                                  ; preds = %assert.exit.L23
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !76
  call void @exit(i32 1), !dbg !76
  unreachable, !dbg !76

assert.exit.L25:                                  ; preds = %assert.exit.L23
  store i32 321, ptr %7, align 4, !dbg !77
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %7), !dbg !77
  store i32 -99, ptr %8, align 4, !dbg !78
  call void @_ZN6VectorIiE8pushBackERKi(ptr noundef nonnull align 8 dereferenceable(32) %vi, ptr %8), !dbg !78
  %47 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !79
  br i1 %47, label %assert.exit.L30, label %assert.then.L30, !dbg !79, !prof !43

assert.then.L30:                                  ; preds = %assert.exit.L25
  %48 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !79
  call void @exit(i32 1), !dbg !79
  unreachable, !dbg !79

assert.exit.L30:                                  ; preds = %assert.exit.L25
  call void @_Z13op.minusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 3), !dbg !80
  %49 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !81
  %50 = load i32, ptr %49, align 4, !dbg !82
  %51 = icmp eq i32 %50, 123, !dbg !82
  br i1 %51, label %assert.exit.L34, label %assert.then.L34, !dbg !82, !prof !43

assert.then.L34:                                  ; preds = %assert.exit.L30
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !82
  call void @exit(i32 1), !dbg !82
  unreachable, !dbg !82

assert.exit.L34:                                  ; preds = %assert.exit.L30
  %53 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !83
  br i1 %53, label %assert.exit.L35, label %assert.then.L35, !dbg !83, !prof !43

assert.then.L35:                                  ; preds = %assert.exit.L34
  %54 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !83
  call void @exit(i32 1), !dbg !83
  unreachable, !dbg !83

assert.exit.L35:                                  ; preds = %assert.exit.L34
  %55 = load %struct.VectorIterator, ptr %it, align 8, !dbg !84
  call void @_Z16op.plusplus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !84
  %56 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !85
  %57 = load i32, ptr %56, align 4, !dbg !86
  %58 = icmp eq i32 %57, 4321, !dbg !86
  br i1 %58, label %assert.exit.L37, label %assert.then.L37, !dbg !86, !prof !43

assert.then.L37:                                  ; preds = %assert.exit.L35
  %59 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !86
  call void @exit(i32 1), !dbg !86
  unreachable, !dbg !86

assert.exit.L37:                                  ; preds = %assert.exit.L35
  %60 = load %struct.VectorIterator, ptr %it, align 8, !dbg !87
  call void @_Z18op.minusminus.postIiEvR14VectorIteratorIiE(ptr %it), !dbg !87
  %61 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !88
  %62 = load i32, ptr %61, align 4, !dbg !89
  %63 = icmp eq i32 %62, 123, !dbg !89
  br i1 %63, label %assert.exit.L39, label %assert.then.L39, !dbg !89, !prof !43

assert.then.L39:                                  ; preds = %assert.exit.L37
  %64 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !89
  call void @exit(i32 1), !dbg !89
  unreachable, !dbg !89

assert.exit.L39:                                  ; preds = %assert.exit.L37
  call void @_Z12op.plusequalIiiEvR14VectorIteratorIiEi(ptr %it, i32 4), !dbg !90
  %65 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !91
  %66 = load i32, ptr %65, align 4, !dbg !92
  %67 = icmp eq i32 %66, -99, !dbg !92
  br i1 %67, label %assert.exit.L41, label %assert.then.L41, !dbg !92, !prof !43

assert.then.L41:                                  ; preds = %assert.exit.L39
  %68 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !92
  call void @exit(i32 1), !dbg !92
  unreachable, !dbg !92

assert.exit.L41:                                  ; preds = %assert.exit.L39
  call void @_ZN14VectorIteratorIiE4nextEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !93
  %69 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr noundef nonnull align 8 dereferenceable(24) %it), !dbg !94
  %70 = xor i1 %69, true, !dbg !94
  store i1 %70, ptr %9, align 1, !dbg !94
  br i1 %70, label %assert.exit.L43, label %assert.then.L43, !dbg !94, !prof !43

assert.then.L43:                                  ; preds = %assert.exit.L41
  %71 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !94
  call void @exit(i32 1), !dbg !94
  unreachable, !dbg !94

assert.exit.L43:                                  ; preds = %assert.exit.L41
  %72 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !95
  call void @llvm.dbg.declare(metadata ptr %item, metadata !97, metadata !DIExpression()), !dbg !98
  store %struct.VectorIterator %72, ptr %10, align 8, !dbg !95
  br label %foreach.head.L46, !dbg !98

foreach.head.L46:                                 ; preds = %foreach.tail.L46, %assert.exit.L43
  %73 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %10), !dbg !98
  br i1 %73, label %foreach.body.L46, label %foreach.exit.L46, !dbg !98

foreach.body.L46:                                 ; preds = %foreach.head.L46
  %74 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %10), !dbg !98
  %75 = load i32, ptr %74, align 4, !dbg !98
  store i32 %75, ptr %item, align 4, !dbg !98
  %76 = load i32, ptr %item, align 4, !dbg !99
  %77 = add nsw i32 %76, 1, !dbg !99
  store i32 %77, ptr %item, align 4, !dbg !99
  br label %foreach.tail.L46, !dbg !99

foreach.tail.L46:                                 ; preds = %foreach.body.L46
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %10), !dbg !99
  br label %foreach.head.L46, !dbg !99

foreach.exit.L46:                                 ; preds = %foreach.head.L46
  %78 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !100
  %79 = load i32, ptr %78, align 4, !dbg !101
  %80 = icmp eq i32 %79, 123, !dbg !101
  br i1 %80, label %assert.exit.L49, label %assert.then.L49, !dbg !101, !prof !43

assert.then.L49:                                  ; preds = %foreach.exit.L46
  %81 = call i32 (ptr, ...) @printf(ptr @anon.string.16), !dbg !101
  call void @exit(i32 1), !dbg !101
  unreachable, !dbg !101

assert.exit.L49:                                  ; preds = %foreach.exit.L46
  %82 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !102
  %83 = load i32, ptr %82, align 4, !dbg !103
  %84 = icmp eq i32 %83, 4321, !dbg !103
  br i1 %84, label %assert.exit.L50, label %assert.then.L50, !dbg !103, !prof !43

assert.then.L50:                                  ; preds = %assert.exit.L49
  %85 = call i32 (ptr, ...) @printf(ptr @anon.string.17), !dbg !103
  call void @exit(i32 1), !dbg !103
  unreachable, !dbg !103

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %86 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !104
  %87 = load i32, ptr %86, align 4, !dbg !105
  %88 = icmp eq i32 %87, 9876, !dbg !105
  br i1 %88, label %assert.exit.L51, label %assert.then.L51, !dbg !105, !prof !43

assert.then.L51:                                  ; preds = %assert.exit.L50
  %89 = call i32 (ptr, ...) @printf(ptr @anon.string.18), !dbg !105
  call void @exit(i32 1), !dbg !105
  unreachable, !dbg !105

assert.exit.L51:                                  ; preds = %assert.exit.L50
  %90 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !106
  call void @llvm.dbg.declare(metadata ptr %item1, metadata !108, metadata !DIExpression()), !dbg !109
  store %struct.VectorIterator %90, ptr %11, align 8, !dbg !106
  br label %foreach.head.L54, !dbg !109

foreach.head.L54:                                 ; preds = %foreach.tail.L54, %assert.exit.L51
  %91 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %11), !dbg !109
  br i1 %91, label %foreach.body.L54, label %foreach.exit.L54, !dbg !109

foreach.body.L54:                                 ; preds = %foreach.head.L54
  %92 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %11), !dbg !109
  store ptr %92, ptr %12, align 8, !dbg !109
  %93 = load ptr, ptr %12, align 8, !dbg !110
  %94 = load i32, ptr %93, align 4, !dbg !110
  %95 = add nsw i32 %94, 1, !dbg !110
  store i32 %95, ptr %93, align 4, !dbg !110
  br label %foreach.tail.L54, !dbg !110

foreach.tail.L54:                                 ; preds = %foreach.body.L54
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %11), !dbg !110
  br label %foreach.head.L54, !dbg !110

foreach.exit.L54:                                 ; preds = %foreach.head.L54
  %96 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !111
  %97 = load i32, ptr %96, align 4, !dbg !112
  %98 = icmp eq i32 %97, 124, !dbg !112
  br i1 %98, label %assert.exit.L57, label %assert.then.L57, !dbg !112, !prof !43

assert.then.L57:                                  ; preds = %foreach.exit.L54
  %99 = call i32 (ptr, ...) @printf(ptr @anon.string.19), !dbg !112
  call void @exit(i32 1), !dbg !112
  unreachable, !dbg !112

assert.exit.L57:                                  ; preds = %foreach.exit.L54
  %100 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !113
  %101 = load i32, ptr %100, align 4, !dbg !114
  %102 = icmp eq i32 %101, 4322, !dbg !114
  br i1 %102, label %assert.exit.L58, label %assert.then.L58, !dbg !114, !prof !43

assert.then.L58:                                  ; preds = %assert.exit.L57
  %103 = call i32 (ptr, ...) @printf(ptr @anon.string.20), !dbg !114
  call void @exit(i32 1), !dbg !114
  unreachable, !dbg !114

assert.exit.L58:                                  ; preds = %assert.exit.L57
  %104 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !115
  %105 = load i32, ptr %104, align 4, !dbg !116
  %106 = icmp eq i32 %105, 9877, !dbg !116
  br i1 %106, label %assert.exit.L59, label %assert.then.L59, !dbg !116, !prof !43

assert.then.L59:                                  ; preds = %assert.exit.L58
  %107 = call i32 (ptr, ...) @printf(ptr @anon.string.21), !dbg !116
  call void @exit(i32 1), !dbg !116
  unreachable, !dbg !116

assert.exit.L59:                                  ; preds = %assert.exit.L58
  %108 = call %struct.VectorIterator @_ZN6VectorIiE11getIteratorEv(ptr noundef nonnull align 8 dereferenceable(32) %vi), !dbg !117
  store %struct.VectorIterator %108, ptr %13, align 8, !dbg !117
  call void @llvm.dbg.declare(metadata ptr %idx, metadata !119, metadata !DIExpression()), !dbg !121
  call void @llvm.dbg.declare(metadata ptr %item2, metadata !122, metadata !DIExpression()), !dbg !123
  store i64 0, ptr %idx, align 8, !dbg !121
  br label %foreach.head.L61, !dbg !123

foreach.head.L61:                                 ; preds = %foreach.tail.L61, %assert.exit.L59
  %109 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %13), !dbg !123
  br i1 %109, label %foreach.body.L61, label %foreach.exit.L61, !dbg !123

foreach.body.L61:                                 ; preds = %foreach.head.L61
  %pair3 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr %13), !dbg !123
  store %struct.Pair %pair3, ptr %pair_addr, align 8, !dbg !123
  %110 = load i64, ptr %pair_addr, align 8, !dbg !123
  store i64 %110, ptr %idx, align 8, !dbg !123
  %item_addr = getelementptr inbounds %struct.Pair, ptr %pair_addr, i32 0, i32 1, !dbg !123
  %111 = load ptr, ptr %item_addr, align 8, !dbg !123
  store ptr %111, ptr %14, align 8, !dbg !123
  %112 = load i64, ptr %idx, align 8, !dbg !124
  %113 = trunc i64 %112 to i32, !dbg !124
  %114 = load ptr, ptr %14, align 8, !dbg !124
  %115 = load i32, ptr %114, align 4, !dbg !124
  %116 = add nsw i32 %115, %113, !dbg !124
  store i32 %116, ptr %114, align 4, !dbg !124
  br label %foreach.tail.L61, !dbg !124

foreach.tail.L61:                                 ; preds = %foreach.body.L61
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %13), !dbg !124
  br label %foreach.head.L61, !dbg !124

foreach.exit.L61:                                 ; preds = %foreach.head.L61
  %117 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 0), !dbg !125
  %118 = load i32, ptr %117, align 4, !dbg !126
  %119 = icmp eq i32 %118, 124, !dbg !126
  br i1 %119, label %assert.exit.L64, label %assert.then.L64, !dbg !126, !prof !43

assert.then.L64:                                  ; preds = %foreach.exit.L61
  %120 = call i32 (ptr, ...) @printf(ptr @anon.string.22), !dbg !126
  call void @exit(i32 1), !dbg !126
  unreachable, !dbg !126

assert.exit.L64:                                  ; preds = %foreach.exit.L61
  %121 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 1), !dbg !127
  %122 = load i32, ptr %121, align 4, !dbg !128
  %123 = icmp eq i32 %122, 4323, !dbg !128
  br i1 %123, label %assert.exit.L65, label %assert.then.L65, !dbg !128, !prof !43

assert.then.L65:                                  ; preds = %assert.exit.L64
  %124 = call i32 (ptr, ...) @printf(ptr @anon.string.23), !dbg !128
  call void @exit(i32 1), !dbg !128
  unreachable, !dbg !128

assert.exit.L65:                                  ; preds = %assert.exit.L64
  %125 = call ptr @_ZN6VectorIiE3getEj(ptr noundef nonnull align 8 dereferenceable(32) %vi, i32 2), !dbg !129
  %126 = load i32, ptr %125, align 4, !dbg !130
  %127 = icmp eq i32 %126, 9879, !dbg !130
  br i1 %127, label %assert.exit.L66, label %assert.then.L66, !dbg !130, !prof !43

assert.then.L66:                                  ; preds = %assert.exit.L65
  %128 = call i32 (ptr, ...) @printf(ptr @anon.string.24), !dbg !130
  call void @exit(i32 1), !dbg !130
  unreachable, !dbg !130

assert.exit.L66:                                  ; preds = %assert.exit.L65
  %129 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !131
  call void @_ZN6VectorIiE4dtorEv(ptr %vi), !dbg !131
  %130 = load i32, ptr %result, align 4, !dbg !131
  ret i32 %130, !dbg !131
}

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

declare void @_ZN6VectorIiE4ctorEv(ptr)

declare void @_ZN6VectorIiE8pushBackERKi(ptr, ptr)

declare i64 @_ZN6VectorIiE7getSizeEv(ptr)

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

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

declare void @_ZN6VectorIiE4dtorEv(ptr)

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #2 = { nofree nounwind }
attributes #3 = { cold noreturn nounwind }

!llvm.dbg.cu = !{!2}
!llvm.module.flags = !{!7, !8, !9, !10, !11, !12, !13}
!llvm.ident = !{!14}

!0 = !DIGlobalVariableExpression(var: !1, expr: !DIExpression())
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !5, line: 68, type: !6, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "/home/marc/Dokumente/Dev/spice/cmake-build-debug/test/./test-files/irgenerator/debug-info/success-dbg-info-complex/source.spice", directory: "./test-files/irgenerator/debug-info/success-dbg-info-complex")
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
!28 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !29, file: !29, line: 25, size: 256, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !30, identifier: "struct.Vector")
!29 = !DIFile(filename: "vector.spice", directory: "/home/marc/Dokumente/Dev/spice/std/data")
!30 = !{!31, !33, !35}
!31 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !28, file: !29, line: 26, baseType: !32, size: 64, offset: 64)
!32 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64)
!33 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !28, file: !29, line: 27, baseType: !34, size: 64, offset: 128)
!34 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !28, file: !29, line: 28, baseType: !34, size: 64, offset: 192)
!36 = !DILocation(line: 6, column: 5, scope: !15)
!37 = !DILocation(line: 6, column: 22, scope: !15)
!38 = !DILocation(line: 7, column: 17, scope: !15)
!39 = !DILocation(line: 8, column: 17, scope: !15)
!40 = !DILocation(line: 9, column: 17, scope: !15)
!41 = !DILocation(line: 10, column: 12, scope: !15)
!42 = !DILocation(line: 10, column: 28, scope: !15)
!43 = !{!"branch_weights", i32 2000, i32 1}
!44 = !DILocation(line: 13, column: 14, scope: !15)
!45 = !DILocalVariable(name: "it", scope: !15, file: !5, line: 13, type: !46)
!46 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator", scope: !29, file: !29, line: 253, size: 192, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !47, identifier: "struct.VectorIterator")
!47 = !{!48, !50}
!48 = !DIDerivedType(tag: DW_TAG_member, name: "vector", scope: !46, file: !29, line: 254, baseType: !49, size: 64, offset: 64)
!49 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !28, size: 64)
!50 = !DIDerivedType(tag: DW_TAG_member, name: "cursor", scope: !46, file: !29, line: 255, baseType: !34, size: 64, offset: 128)
!51 = !DILocation(line: 13, column: 5, scope: !15)
!52 = !DILocation(line: 14, column: 12, scope: !15)
!53 = !DILocation(line: 15, column: 12, scope: !15)
!54 = !DILocation(line: 15, column: 24, scope: !15)
!55 = !DILocation(line: 16, column: 12, scope: !15)
!56 = !DILocation(line: 16, column: 24, scope: !15)
!57 = !DILocation(line: 17, column: 5, scope: !15)
!58 = !DILocation(line: 18, column: 12, scope: !15)
!59 = !DILocation(line: 18, column: 24, scope: !15)
!60 = !DILocation(line: 19, column: 12, scope: !15)
!61 = !DILocation(line: 20, column: 5, scope: !15)
!62 = !DILocation(line: 21, column: 16, scope: !15)
!63 = !DILocalVariable(name: "pair", scope: !15, file: !5, line: 21, type: !64)
!64 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair", scope: !65, file: !65, line: 8, size: 128, align: 8, flags: DIFlagTypePassByReference | DIFlagNonTrivial, elements: !66, identifier: "struct.Pair")
!65 = !DIFile(filename: "pair.spice", directory: "/home/marc/Dokumente/Dev/spice/std/data")
!66 = !{!67, !68}
!67 = !DIDerivedType(tag: DW_TAG_member, name: "first", scope: !64, file: !65, line: 9, baseType: !34, size: 64)
!68 = !DIDerivedType(tag: DW_TAG_member, name: "second", scope: !64, file: !65, line: 10, baseType: !69, size: 64, offset: 64)
!69 = !DIDerivedType(tag: DW_TAG_reference_type, baseType: !18, size: 64)
!70 = !DILocation(line: 21, column: 5, scope: !15)
!71 = !DILocation(line: 22, column: 12, scope: !15)
!72 = !DILocation(line: 22, column: 31, scope: !15)
!73 = !DILocation(line: 23, column: 12, scope: !15)
!74 = !DILocation(line: 23, column: 32, scope: !15)
!75 = !DILocation(line: 24, column: 5, scope: !15)
!76 = !DILocation(line: 25, column: 13, scope: !15)
!77 = !DILocation(line: 28, column: 17, scope: !15)
!78 = !DILocation(line: 29, column: 17, scope: !15)
!79 = !DILocation(line: 30, column: 12, scope: !15)
!80 = !DILocation(line: 33, column: 5, scope: !15)
!81 = !DILocation(line: 34, column: 12, scope: !15)
!82 = !DILocation(line: 34, column: 24, scope: !15)
!83 = !DILocation(line: 35, column: 12, scope: !15)
!84 = !DILocation(line: 36, column: 5, scope: !15)
!85 = !DILocation(line: 37, column: 12, scope: !15)
!86 = !DILocation(line: 37, column: 24, scope: !15)
!87 = !DILocation(line: 38, column: 5, scope: !15)
!88 = !DILocation(line: 39, column: 12, scope: !15)
!89 = !DILocation(line: 39, column: 24, scope: !15)
!90 = !DILocation(line: 40, column: 5, scope: !15)
!91 = !DILocation(line: 41, column: 12, scope: !15)
!92 = !DILocation(line: 41, column: 24, scope: !15)
!93 = !DILocation(line: 42, column: 5, scope: !15)
!94 = !DILocation(line: 43, column: 13, scope: !15)
!95 = !DILocation(line: 46, column: 24, scope: !96)
!96 = distinct !DILexicalBlock(scope: !15, file: !5, line: 46, column: 5)
!97 = !DILocalVariable(name: "item", scope: !96, file: !5, line: 46, type: !18)
!98 = !DILocation(line: 46, column: 13, scope: !96)
!99 = !DILocation(line: 47, column: 9, scope: !96)
!100 = !DILocation(line: 49, column: 19, scope: !15)
!101 = !DILocation(line: 49, column: 25, scope: !15)
!102 = !DILocation(line: 50, column: 19, scope: !15)
!103 = !DILocation(line: 50, column: 25, scope: !15)
!104 = !DILocation(line: 51, column: 19, scope: !15)
!105 = !DILocation(line: 51, column: 25, scope: !15)
!106 = !DILocation(line: 54, column: 25, scope: !107)
!107 = distinct !DILexicalBlock(scope: !15, file: !5, line: 54, column: 5)
!108 = !DILocalVariable(name: "item", scope: !107, file: !5, line: 54, type: !69)
!109 = !DILocation(line: 54, column: 13, scope: !107)
!110 = !DILocation(line: 55, column: 9, scope: !107)
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
!122 = !DILocalVariable(name: "item", scope: !118, file: !5, line: 61, type: !69)
!123 = !DILocation(line: 61, column: 23, scope: !118)
!124 = !DILocation(line: 62, column: 9, scope: !118)
!125 = !DILocation(line: 64, column: 19, scope: !15)
!126 = !DILocation(line: 64, column: 25, scope: !15)
!127 = !DILocation(line: 65, column: 19, scope: !15)
!128 = !DILocation(line: 65, column: 25, scope: !15)
!129 = !DILocation(line: 66, column: 19, scope: !15)
!130 = !DILocation(line: 66, column: 25, scope: !15)
!131 = !DILocation(line: 68, column: 5, scope: !15)
