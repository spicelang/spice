; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Vector = type { ptr, i64, i64 }
%struct.VectorIterator = type { ptr, i64 }
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
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1, !dbg !0

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main(i32 %0, ptr %1) #0 !dbg !15 {
  %result = alloca i32, align 4
  %argc = alloca i32, align 4
  %argv = alloca ptr, align 8
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
  call void @llvm.dbg.declare(metadata ptr %argc, metadata !25, metadata !DIExpression()), !dbg !24
  store i32 %0, ptr %argc, align 4, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %argv, metadata !26, metadata !DIExpression()), !dbg !24
  store ptr %1, ptr %argv, align 8, !dbg !24
  call void @llvm.dbg.declare(metadata ptr %vi, metadata !27, metadata !DIExpression()), !dbg !36
  call void @_ZN6VectorIiE4ctorEv(ptr %vi), !dbg !37
  store i32 123, ptr %3, align 4, !dbg !38
  call void @_ZN6VectorIiE8pushBackERKi(ptr %vi, ptr %3), !dbg !38
  store i32 4321, ptr %4, align 4, !dbg !39
  call void @_ZN6VectorIiE8pushBackERKi(ptr %vi, ptr %4), !dbg !39
  store i32 9876, ptr %5, align 4, !dbg !40
  call void @_ZN6VectorIiE8pushBackERKi(ptr %vi, ptr %5), !dbg !40
  %15 = call i64 @_ZN6VectorIiE7getSizeEv(ptr %vi), !dbg !41
  %16 = icmp eq i64 %15, 3, !dbg !42
  br i1 %16, label %assert.exit.L9, label %assert.then.L9, !dbg !42, !prof !43

assert.then.L9:                                   ; preds = %2
  %17 = call i32 (ptr, ...) @printf(ptr @anon.string.0), !dbg !42
  call void @exit(i32 1), !dbg !42
  unreachable, !dbg !42

assert.exit.L9:                                   ; preds = %2
  %18 = call %struct.VectorIterator @"_Z7iterateR35std/iterator/vector-iterator.VectorIiE"(ptr %vi), !dbg !44
  store %struct.VectorIterator %18, ptr %it, align 8, !dbg !44
  call void @llvm.dbg.declare(metadata ptr %it, metadata !45, metadata !DIExpression()), !dbg !52
  store %struct.VectorIterator %18, ptr %it, align 8, !dbg !44
  %19 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %it), !dbg !53
  br i1 %19, label %assert.exit.L13, label %assert.then.L13, !dbg !53, !prof !43

assert.then.L13:                                  ; preds = %assert.exit.L9
  %20 = call i32 (ptr, ...) @printf(ptr @anon.string.1), !dbg !53
  call void @exit(i32 1), !dbg !53
  unreachable, !dbg !53

assert.exit.L13:                                  ; preds = %assert.exit.L9
  %21 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %it), !dbg !54
  %22 = load i32, ptr %21, align 4, !dbg !55
  %23 = icmp eq i32 %22, 123, !dbg !55
  br i1 %23, label %assert.exit.L14, label %assert.then.L14, !dbg !55, !prof !43

assert.then.L14:                                  ; preds = %assert.exit.L13
  %24 = call i32 (ptr, ...) @printf(ptr @anon.string.2), !dbg !55
  call void @exit(i32 1), !dbg !55
  unreachable, !dbg !55

assert.exit.L14:                                  ; preds = %assert.exit.L13
  %25 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %it), !dbg !56
  %26 = load i32, ptr %25, align 4, !dbg !57
  %27 = icmp eq i32 %26, 123, !dbg !57
  br i1 %27, label %assert.exit.L15, label %assert.then.L15, !dbg !57, !prof !43

assert.then.L15:                                  ; preds = %assert.exit.L14
  %28 = call i32 (ptr, ...) @printf(ptr @anon.string.3), !dbg !57
  call void @exit(i32 1), !dbg !57
  unreachable, !dbg !57

assert.exit.L15:                                  ; preds = %assert.exit.L14
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %it), !dbg !58
  %29 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %it), !dbg !59
  %30 = load i32, ptr %29, align 4, !dbg !60
  %31 = icmp eq i32 %30, 4321, !dbg !60
  br i1 %31, label %assert.exit.L17, label %assert.then.L17, !dbg !60, !prof !43

assert.then.L17:                                  ; preds = %assert.exit.L15
  %32 = call i32 (ptr, ...) @printf(ptr @anon.string.4), !dbg !60
  call void @exit(i32 1), !dbg !60
  unreachable, !dbg !60

assert.exit.L17:                                  ; preds = %assert.exit.L15
  %33 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %it), !dbg !61
  br i1 %33, label %assert.exit.L18, label %assert.then.L18, !dbg !61, !prof !43

assert.then.L18:                                  ; preds = %assert.exit.L17
  %34 = call i32 (ptr, ...) @printf(ptr @anon.string.5), !dbg !61
  call void @exit(i32 1), !dbg !61
  unreachable, !dbg !61

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %it), !dbg !62
  %35 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr %it), !dbg !63
  store %struct.Pair %35, ptr %pair, align 8, !dbg !63
  call void @llvm.dbg.declare(metadata ptr %pair, metadata !64, metadata !DIExpression()), !dbg !70
  store %struct.Pair %35, ptr %pair, align 8, !dbg !63
  %36 = call ptr @_ZN4PairImRiE8getFirstEv(ptr %pair), !dbg !71
  %37 = load i64, ptr %36, align 8, !dbg !72
  %38 = icmp eq i64 %37, 2, !dbg !72
  br i1 %38, label %assert.exit.L21, label %assert.then.L21, !dbg !72, !prof !43

assert.then.L21:                                  ; preds = %assert.exit.L18
  %39 = call i32 (ptr, ...) @printf(ptr @anon.string.6), !dbg !72
  call void @exit(i32 1), !dbg !72
  unreachable, !dbg !72

assert.exit.L21:                                  ; preds = %assert.exit.L18
  %40 = call ptr @_ZN4PairImRiE9getSecondEv(ptr %pair), !dbg !73
  %41 = load i32, ptr %40, align 4, !dbg !74
  %42 = icmp eq i32 %41, 9876, !dbg !74
  br i1 %42, label %assert.exit.L22, label %assert.then.L22, !dbg !74, !prof !43

assert.then.L22:                                  ; preds = %assert.exit.L21
  %43 = call i32 (ptr, ...) @printf(ptr @anon.string.7), !dbg !74
  call void @exit(i32 1), !dbg !74
  unreachable, !dbg !74

assert.exit.L22:                                  ; preds = %assert.exit.L21
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %it), !dbg !75
  %44 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %it), !dbg !76
  %45 = xor i1 %44, true, !dbg !76
  store i1 %45, ptr %6, align 1, !dbg !76
  br i1 %45, label %assert.exit.L24, label %assert.then.L24, !dbg !76, !prof !43

assert.then.L24:                                  ; preds = %assert.exit.L22
  %46 = call i32 (ptr, ...) @printf(ptr @anon.string.8), !dbg !76
  call void @exit(i32 1), !dbg !76
  unreachable, !dbg !76

assert.exit.L24:                                  ; preds = %assert.exit.L22
  store i32 321, ptr %7, align 4, !dbg !77
  call void @_ZN6VectorIiE8pushBackERKi(ptr %vi, ptr %7), !dbg !77
  store i32 -99, ptr %8, align 4, !dbg !78
  call void @_ZN6VectorIiE8pushBackERKi(ptr %vi, ptr %8), !dbg !78
  %47 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %it), !dbg !79
  br i1 %47, label %assert.exit.L29, label %assert.then.L29, !dbg !79, !prof !43

assert.then.L29:                                  ; preds = %assert.exit.L24
  %48 = call i32 (ptr, ...) @printf(ptr @anon.string.9), !dbg !79
  call void @exit(i32 1), !dbg !79
  unreachable, !dbg !79

assert.exit.L29:                                  ; preds = %assert.exit.L24
  call void @_Z13op.minusequalR14VectorIteratorIiEi(ptr %it, i32 3), !dbg !80
  %49 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %it), !dbg !81
  %50 = load i32, ptr %49, align 4, !dbg !82
  %51 = icmp eq i32 %50, 123, !dbg !82
  br i1 %51, label %assert.exit.L33, label %assert.then.L33, !dbg !82, !prof !43

assert.then.L33:                                  ; preds = %assert.exit.L29
  %52 = call i32 (ptr, ...) @printf(ptr @anon.string.10), !dbg !82
  call void @exit(i32 1), !dbg !82
  unreachable, !dbg !82

assert.exit.L33:                                  ; preds = %assert.exit.L29
  %53 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %it), !dbg !83
  br i1 %53, label %assert.exit.L34, label %assert.then.L34, !dbg !83, !prof !43

assert.then.L34:                                  ; preds = %assert.exit.L33
  %54 = call i32 (ptr, ...) @printf(ptr @anon.string.11), !dbg !83
  call void @exit(i32 1), !dbg !83
  unreachable, !dbg !83

assert.exit.L34:                                  ; preds = %assert.exit.L33
  %55 = load %struct.VectorIterator, ptr %it, align 8, !dbg !84
  call void @_Z16op.plusplus.postR14VectorIteratorIiE(ptr %it), !dbg !84
  %56 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %it), !dbg !85
  %57 = load i32, ptr %56, align 4, !dbg !86
  %58 = icmp eq i32 %57, 4321, !dbg !86
  br i1 %58, label %assert.exit.L36, label %assert.then.L36, !dbg !86, !prof !43

assert.then.L36:                                  ; preds = %assert.exit.L34
  %59 = call i32 (ptr, ...) @printf(ptr @anon.string.12), !dbg !86
  call void @exit(i32 1), !dbg !86
  unreachable, !dbg !86

assert.exit.L36:                                  ; preds = %assert.exit.L34
  %60 = load %struct.VectorIterator, ptr %it, align 8, !dbg !87
  call void @_Z18op.minusminus.postR14VectorIteratorIiE(ptr %it), !dbg !87
  %61 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %it), !dbg !88
  %62 = load i32, ptr %61, align 4, !dbg !89
  %63 = icmp eq i32 %62, 123, !dbg !89
  br i1 %63, label %assert.exit.L38, label %assert.then.L38, !dbg !89, !prof !43

assert.then.L38:                                  ; preds = %assert.exit.L36
  %64 = call i32 (ptr, ...) @printf(ptr @anon.string.13), !dbg !89
  call void @exit(i32 1), !dbg !89
  unreachable, !dbg !89

assert.exit.L38:                                  ; preds = %assert.exit.L36
  call void @_Z12op.plusequalR14VectorIteratorIiEi(ptr %it, i32 4), !dbg !90
  %65 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %it), !dbg !91
  %66 = load i32, ptr %65, align 4, !dbg !92
  %67 = icmp eq i32 %66, -99, !dbg !92
  br i1 %67, label %assert.exit.L40, label %assert.then.L40, !dbg !92, !prof !43

assert.then.L40:                                  ; preds = %assert.exit.L38
  %68 = call i32 (ptr, ...) @printf(ptr @anon.string.14), !dbg !92
  call void @exit(i32 1), !dbg !92
  unreachable, !dbg !92

assert.exit.L40:                                  ; preds = %assert.exit.L38
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %it), !dbg !93
  %69 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %it), !dbg !94
  %70 = xor i1 %69, true, !dbg !94
  store i1 %70, ptr %9, align 1, !dbg !94
  br i1 %70, label %assert.exit.L42, label %assert.then.L42, !dbg !94, !prof !43

assert.then.L42:                                  ; preds = %assert.exit.L40
  %71 = call i32 (ptr, ...) @printf(ptr @anon.string.15), !dbg !94
  call void @exit(i32 1), !dbg !94
  unreachable, !dbg !94

assert.exit.L42:                                  ; preds = %assert.exit.L40
  %72 = call %struct.VectorIterator @"_Z7iterateR35std/iterator/vector-iterator.VectorIiE"(ptr %vi), !dbg !95
  call void @llvm.dbg.declare(metadata ptr %item, metadata !96, metadata !DIExpression()), !dbg !97
  store %struct.VectorIterator %72, ptr %10, align 8, !dbg !95
  br label %foreach.head.L45, !dbg !97

foreach.head.L45:                                 ; preds = %foreach.tail.L45, %assert.exit.L42
  %73 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %10), !dbg !97
  br i1 %73, label %foreach.body.L45, label %foreach.exit.L45, !dbg !97

foreach.body.L45:                                 ; preds = %foreach.head.L45
  %74 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %10), !dbg !97
  %75 = load i32, ptr %74, align 4, !dbg !97
  store i32 %75, ptr %item, align 4, !dbg !97
  %76 = load i32, ptr %item, align 4, !dbg !98
  %77 = add i32 %76, 1, !dbg !98
  store i32 %77, ptr %item, align 4, !dbg !98
  br label %foreach.tail.L45, !dbg !98

foreach.tail.L45:                                 ; preds = %foreach.body.L45
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %10), !dbg !98
  br label %foreach.head.L45, !dbg !98

foreach.exit.L45:                                 ; preds = %foreach.head.L45
  %78 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 0), !dbg !99
  %79 = load i32, ptr %78, align 4, !dbg !100
  %80 = icmp eq i32 %79, 123, !dbg !100
  br i1 %80, label %assert.exit.L48, label %assert.then.L48, !dbg !100, !prof !43

assert.then.L48:                                  ; preds = %foreach.exit.L45
  %81 = call i32 (ptr, ...) @printf(ptr @anon.string.16), !dbg !100
  call void @exit(i32 1), !dbg !100
  unreachable, !dbg !100

assert.exit.L48:                                  ; preds = %foreach.exit.L45
  %82 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 1), !dbg !101
  %83 = load i32, ptr %82, align 4, !dbg !102
  %84 = icmp eq i32 %83, 4321, !dbg !102
  br i1 %84, label %assert.exit.L49, label %assert.then.L49, !dbg !102, !prof !43

assert.then.L49:                                  ; preds = %assert.exit.L48
  %85 = call i32 (ptr, ...) @printf(ptr @anon.string.17), !dbg !102
  call void @exit(i32 1), !dbg !102
  unreachable, !dbg !102

assert.exit.L49:                                  ; preds = %assert.exit.L48
  %86 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 2), !dbg !103
  %87 = load i32, ptr %86, align 4, !dbg !104
  %88 = icmp eq i32 %87, 9876, !dbg !104
  br i1 %88, label %assert.exit.L50, label %assert.then.L50, !dbg !104, !prof !43

assert.then.L50:                                  ; preds = %assert.exit.L49
  %89 = call i32 (ptr, ...) @printf(ptr @anon.string.18), !dbg !104
  call void @exit(i32 1), !dbg !104
  unreachable, !dbg !104

assert.exit.L50:                                  ; preds = %assert.exit.L49
  %90 = call %struct.VectorIterator @"_Z7iterateR35std/iterator/vector-iterator.VectorIiE"(ptr %vi), !dbg !105
  call void @llvm.dbg.declare(metadata ptr %item1, metadata !106, metadata !DIExpression()), !dbg !107
  store %struct.VectorIterator %90, ptr %11, align 8, !dbg !105
  br label %foreach.head.L53, !dbg !107

foreach.head.L53:                                 ; preds = %foreach.tail.L53, %assert.exit.L50
  %91 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %11), !dbg !107
  br i1 %91, label %foreach.body.L53, label %foreach.exit.L53, !dbg !107

foreach.body.L53:                                 ; preds = %foreach.head.L53
  %92 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr %11), !dbg !107
  store ptr %92, ptr %12, align 8, !dbg !107
  %93 = load ptr, ptr %12, align 8, !dbg !108
  %94 = load i32, ptr %93, align 4, !dbg !108
  %95 = add i32 %94, 1, !dbg !108
  store i32 %95, ptr %93, align 4, !dbg !108
  br label %foreach.tail.L53, !dbg !108

foreach.tail.L53:                                 ; preds = %foreach.body.L53
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %11), !dbg !108
  br label %foreach.head.L53, !dbg !108

foreach.exit.L53:                                 ; preds = %foreach.head.L53
  %96 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 0), !dbg !109
  %97 = load i32, ptr %96, align 4, !dbg !110
  %98 = icmp eq i32 %97, 124, !dbg !110
  br i1 %98, label %assert.exit.L56, label %assert.then.L56, !dbg !110, !prof !43

assert.then.L56:                                  ; preds = %foreach.exit.L53
  %99 = call i32 (ptr, ...) @printf(ptr @anon.string.19), !dbg !110
  call void @exit(i32 1), !dbg !110
  unreachable, !dbg !110

assert.exit.L56:                                  ; preds = %foreach.exit.L53
  %100 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 1), !dbg !111
  %101 = load i32, ptr %100, align 4, !dbg !112
  %102 = icmp eq i32 %101, 4322, !dbg !112
  br i1 %102, label %assert.exit.L57, label %assert.then.L57, !dbg !112, !prof !43

assert.then.L57:                                  ; preds = %assert.exit.L56
  %103 = call i32 (ptr, ...) @printf(ptr @anon.string.20), !dbg !112
  call void @exit(i32 1), !dbg !112
  unreachable, !dbg !112

assert.exit.L57:                                  ; preds = %assert.exit.L56
  %104 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 2), !dbg !113
  %105 = load i32, ptr %104, align 4, !dbg !114
  %106 = icmp eq i32 %105, 9877, !dbg !114
  br i1 %106, label %assert.exit.L58, label %assert.then.L58, !dbg !114, !prof !43

assert.then.L58:                                  ; preds = %assert.exit.L57
  %107 = call i32 (ptr, ...) @printf(ptr @anon.string.21), !dbg !114
  call void @exit(i32 1), !dbg !114
  unreachable, !dbg !114

assert.exit.L58:                                  ; preds = %assert.exit.L57
  %108 = call %struct.VectorIterator @"_Z7iterateR35std/iterator/vector-iterator.VectorIiE"(ptr %vi), !dbg !115
  store %struct.VectorIterator %108, ptr %13, align 8, !dbg !115
  call void @llvm.dbg.declare(metadata ptr %idx, metadata !116, metadata !DIExpression()), !dbg !118
  call void @llvm.dbg.declare(metadata ptr %item2, metadata !119, metadata !DIExpression()), !dbg !120
  store i64 0, ptr %idx, align 8, !dbg !118
  br label %foreach.head.L60, !dbg !120

foreach.head.L60:                                 ; preds = %foreach.tail.L60, %assert.exit.L58
  %109 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr %13), !dbg !120
  br i1 %109, label %foreach.body.L60, label %foreach.exit.L60, !dbg !120

foreach.body.L60:                                 ; preds = %foreach.head.L60
  %pair3 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr %13), !dbg !120
  store %struct.Pair %pair3, ptr %pair_addr, align 8, !dbg !120
  %idx_addr = getelementptr inbounds %struct.Pair, ptr %pair_addr, i32 0, i32 0, !dbg !120
  %110 = load i64, ptr %idx_addr, align 8, !dbg !120
  store i64 %110, ptr %idx, align 8, !dbg !120
  %item_addr = getelementptr inbounds %struct.Pair, ptr %pair_addr, i32 0, i32 1, !dbg !120
  %111 = load ptr, ptr %item_addr, align 8, !dbg !120
  store ptr %111, ptr %14, align 8, !dbg !120
  %112 = load i64, ptr %idx, align 8, !dbg !121
  %113 = trunc i64 %112 to i32, !dbg !121
  %114 = load ptr, ptr %14, align 8, !dbg !121
  %115 = load i32, ptr %114, align 4, !dbg !121
  %116 = add i32 %115, %113, !dbg !121
  store i32 %116, ptr %114, align 4, !dbg !121
  br label %foreach.tail.L60, !dbg !121

foreach.tail.L60:                                 ; preds = %foreach.body.L60
  call void @_ZN14VectorIteratorIiE4nextEv(ptr %13), !dbg !121
  br label %foreach.head.L60, !dbg !121

foreach.exit.L60:                                 ; preds = %foreach.head.L60
  %117 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 0), !dbg !122
  %118 = load i32, ptr %117, align 4, !dbg !123
  %119 = icmp eq i32 %118, 124, !dbg !123
  br i1 %119, label %assert.exit.L63, label %assert.then.L63, !dbg !123, !prof !43

assert.then.L63:                                  ; preds = %foreach.exit.L60
  %120 = call i32 (ptr, ...) @printf(ptr @anon.string.22), !dbg !123
  call void @exit(i32 1), !dbg !123
  unreachable, !dbg !123

assert.exit.L63:                                  ; preds = %foreach.exit.L60
  %121 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 1), !dbg !124
  %122 = load i32, ptr %121, align 4, !dbg !125
  %123 = icmp eq i32 %122, 4323, !dbg !125
  br i1 %123, label %assert.exit.L64, label %assert.then.L64, !dbg !125, !prof !43

assert.then.L64:                                  ; preds = %assert.exit.L63
  %124 = call i32 (ptr, ...) @printf(ptr @anon.string.23), !dbg !125
  call void @exit(i32 1), !dbg !125
  unreachable, !dbg !125

assert.exit.L64:                                  ; preds = %assert.exit.L63
  %125 = call ptr @_ZN6VectorIiE3getEj(ptr %vi, i32 2), !dbg !126
  %126 = load i32, ptr %125, align 4, !dbg !127
  %127 = icmp eq i32 %126, 9879, !dbg !127
  br i1 %127, label %assert.exit.L65, label %assert.then.L65, !dbg !127, !prof !43

assert.then.L65:                                  ; preds = %assert.exit.L64
  %128 = call i32 (ptr, ...) @printf(ptr @anon.string.24), !dbg !127
  call void @exit(i32 1), !dbg !127
  unreachable, !dbg !127

assert.exit.L65:                                  ; preds = %assert.exit.L64
  %129 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0), !dbg !128
  call void @_ZN6VectorIiE4dtorEv(ptr %vi), !dbg !128
  %130 = load i32, ptr %result, align 4, !dbg !128
  ret i32 %130, !dbg !128
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

declare %struct.VectorIterator @"_Z7iterateR35std/iterator/vector-iterator.VectorIiE"(ptr)

declare i1 @_ZN14VectorIteratorIiE7isValidEv(ptr)

declare ptr @_ZN14VectorIteratorIiE3getEv(ptr)

declare void @_ZN14VectorIteratorIiE4nextEv(ptr)

declare %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr)

declare ptr @_ZN4PairImRiE8getFirstEv(ptr)

declare ptr @_ZN4PairImRiE9getSecondEv(ptr)

declare void @_Z13op.minusequalR14VectorIteratorIiEi(ptr, i32)

declare void @_Z16op.plusplus.postR14VectorIteratorIiE(ptr)

declare void @_Z18op.minusminus.postR14VectorIteratorIiE(ptr)

declare void @_Z12op.plusequalR14VectorIteratorIiEi(ptr, i32)

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
!1 = distinct !DIGlobalVariable(name: "printf.str.0", linkageName: "printf.str.0", scope: !2, file: !5, line: 67, type: !6, isLocal: true, isDefinition: true)
!2 = distinct !DICompileUnit(language: 32768, file: !3, producer: "spice version dev (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, globals: !4, splitDebugInlining: false, nameTableKind: None)
!3 = !DIFile(filename: "C:\\Users\\I516467\\Documents\\JustForFunGitHubClones\\spice\\cmake-build-debug\\test\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex\\source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!4 = !{!0}
!5 = !DIFile(filename: "source.spice", directory: ".\\test-files\\irgenerator\\debug-info\\success-dgb-info-complex")
!6 = !DIStringType(name: "printf.str.0", size: 184)
!7 = !{i32 7, !"Dwarf Version", i32 4}
!8 = !{i32 2, !"Debug Info Version", i32 3}
!9 = !{i32 1, !"wchar_size", i32 4}
!10 = !{i32 8, !"PIC Level", i32 2}
!11 = !{i32 7, !"PIE Level", i32 0}
!12 = !{i32 7, !"uwtable", i32 2}
!13 = !{i32 7, !"frame-pointer", i32 2}
!14 = !{!"spice version dev (https://github.com/spicelang/spice)"}
!15 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainiPPc", scope: !5, file: !5, line: 3, type: !16, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !2, retainedNodes: !22)
!16 = !DISubroutineType(types: !17)
!17 = !{!18, !18, !19}
!18 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!19 = !DICompositeType(tag: DW_TAG_array_type, baseType: !20, elements: !22)
!20 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !21, size: 64)
!21 = !DIBasicType(name: "char", size: 8, encoding: DW_ATE_unsigned_char)
!22 = !{}
!23 = !DILocalVariable(name: "result", scope: !15, file: !5, line: 3, type: !18)
!24 = !DILocation(line: 3, column: 1, scope: !15)
!25 = !DILocalVariable(name: "argc", arg: 1, scope: !15, file: !5, line: 3, type: !18)
!26 = !DILocalVariable(name: "argv", arg: 2, scope: !15, file: !5, line: 3, type: !19)
!27 = !DILocalVariable(name: "vi", scope: !15, file: !5, line: 5, type: !28)
!28 = !DICompositeType(tag: DW_TAG_structure_type, name: "Vector", scope: !29, file: !29, line: 27, size: 192, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !30, identifier: "struct.Vector")
!29 = !DIFile(filename: "vector.spice", directory: "C:\\Users\\I516467\\Documents\\JustForFunGitHubClones\\spice\\std\\data")
!30 = !{!31, !33, !35}
!31 = !DIDerivedType(tag: DW_TAG_member, name: "contents", scope: !28, file: !29, line: 28, baseType: !32, size: 64)
!32 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !18, size: 64)
!33 = !DIDerivedType(tag: DW_TAG_member, name: "capacity", scope: !28, file: !29, line: 29, baseType: !34, size: 64, offset: 64)
!34 = !DIBasicType(name: "unsigned long", size: 64, encoding: DW_ATE_unsigned)
!35 = !DIDerivedType(tag: DW_TAG_member, name: "size", scope: !28, file: !29, line: 30, baseType: !34, size: 64, offset: 128)
!36 = !DILocation(line: 5, column: 5, scope: !15)
!37 = !DILocation(line: 5, column: 22, scope: !15)
!38 = !DILocation(line: 6, column: 17, scope: !15)
!39 = !DILocation(line: 7, column: 17, scope: !15)
!40 = !DILocation(line: 8, column: 17, scope: !15)
!41 = !DILocation(line: 9, column: 12, scope: !15)
!42 = !DILocation(line: 9, column: 28, scope: !15)
!43 = !{!"branch_weights", i32 2000, i32 1}
!44 = !DILocation(line: 12, column: 22, scope: !15)
!45 = !DILocalVariable(name: "it", scope: !15, file: !5, line: 12, type: !46)
!46 = !DICompositeType(tag: DW_TAG_structure_type, name: "VectorIterator", scope: !47, file: !47, line: 11, size: 128, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !48, identifier: "struct.VectorIterator")
!47 = !DIFile(filename: "vector-iterator.spice", directory: "C:\\Users\\I516467\\Documents\\JustForFunGitHubClones\\spice\\std\\iterator")
!48 = !{!49, !51}
!49 = !DIDerivedType(tag: DW_TAG_member, name: "vector", scope: !46, file: !47, line: 12, baseType: !50, size: 64)
!50 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !28, size: 64)
!51 = !DIDerivedType(tag: DW_TAG_member, name: "cursor", scope: !46, file: !47, line: 13, baseType: !34, size: 64, offset: 64)
!52 = !DILocation(line: 12, column: 5, scope: !15)
!53 = !DILocation(line: 13, column: 12, scope: !15)
!54 = !DILocation(line: 14, column: 12, scope: !15)
!55 = !DILocation(line: 14, column: 24, scope: !15)
!56 = !DILocation(line: 15, column: 12, scope: !15)
!57 = !DILocation(line: 15, column: 24, scope: !15)
!58 = !DILocation(line: 16, column: 5, scope: !15)
!59 = !DILocation(line: 17, column: 12, scope: !15)
!60 = !DILocation(line: 17, column: 24, scope: !15)
!61 = !DILocation(line: 18, column: 12, scope: !15)
!62 = !DILocation(line: 19, column: 5, scope: !15)
!63 = !DILocation(line: 20, column: 16, scope: !15)
!64 = !DILocalVariable(name: "pair", scope: !15, file: !5, line: 20, type: !65)
!65 = !DICompositeType(tag: DW_TAG_structure_type, name: "Pair", scope: !66, file: !66, line: 8, size: 128, align: 8, flags: DIFlagTypePassByValue | DIFlagNonTrivial, elements: !67, identifier: "struct.Pair")
!66 = !DIFile(filename: "pair.spice", directory: "C:\\Users\\I516467\\Documents\\JustForFunGitHubClones\\spice\\std\\data")
!67 = !{!68, !69}
!68 = !DIDerivedType(tag: DW_TAG_member, name: "first", scope: !65, file: !66, line: 9, baseType: !34, size: 64)
!69 = !DIDerivedType(tag: DW_TAG_member, name: "second", scope: !65, file: !66, line: 10, baseType: !32, size: 64, offset: 64)
!70 = !DILocation(line: 20, column: 5, scope: !15)
!71 = !DILocation(line: 21, column: 12, scope: !15)
!72 = !DILocation(line: 21, column: 31, scope: !15)
!73 = !DILocation(line: 22, column: 12, scope: !15)
!74 = !DILocation(line: 22, column: 32, scope: !15)
!75 = !DILocation(line: 23, column: 5, scope: !15)
!76 = !DILocation(line: 24, column: 13, scope: !15)
!77 = !DILocation(line: 27, column: 17, scope: !15)
!78 = !DILocation(line: 28, column: 17, scope: !15)
!79 = !DILocation(line: 29, column: 12, scope: !15)
!80 = !DILocation(line: 32, column: 5, scope: !15)
!81 = !DILocation(line: 33, column: 12, scope: !15)
!82 = !DILocation(line: 33, column: 24, scope: !15)
!83 = !DILocation(line: 34, column: 12, scope: !15)
!84 = !DILocation(line: 35, column: 5, scope: !15)
!85 = !DILocation(line: 36, column: 12, scope: !15)
!86 = !DILocation(line: 36, column: 24, scope: !15)
!87 = !DILocation(line: 37, column: 5, scope: !15)
!88 = !DILocation(line: 38, column: 12, scope: !15)
!89 = !DILocation(line: 38, column: 24, scope: !15)
!90 = !DILocation(line: 39, column: 5, scope: !15)
!91 = !DILocation(line: 40, column: 12, scope: !15)
!92 = !DILocation(line: 40, column: 24, scope: !15)
!93 = !DILocation(line: 41, column: 5, scope: !15)
!94 = !DILocation(line: 42, column: 13, scope: !15)
!95 = !DILocation(line: 45, column: 32, scope: !15)
!96 = !DILocalVariable(name: "item", scope: !15, file: !5, line: 45, type: !18)
!97 = !DILocation(line: 45, column: 13, scope: !15)
!98 = !DILocation(line: 46, column: 9, scope: !15)
!99 = !DILocation(line: 48, column: 19, scope: !15)
!100 = !DILocation(line: 48, column: 25, scope: !15)
!101 = !DILocation(line: 49, column: 19, scope: !15)
!102 = !DILocation(line: 49, column: 25, scope: !15)
!103 = !DILocation(line: 50, column: 19, scope: !15)
!104 = !DILocation(line: 50, column: 25, scope: !15)
!105 = !DILocation(line: 53, column: 33, scope: !15)
!106 = !DILocalVariable(name: "item", scope: !15, file: !5, line: 53, type: !32)
!107 = !DILocation(line: 53, column: 13, scope: !15)
!108 = !DILocation(line: 54, column: 9, scope: !15)
!109 = !DILocation(line: 56, column: 19, scope: !15)
!110 = !DILocation(line: 56, column: 25, scope: !15)
!111 = !DILocation(line: 57, column: 19, scope: !15)
!112 = !DILocation(line: 57, column: 25, scope: !15)
!113 = !DILocation(line: 58, column: 19, scope: !15)
!114 = !DILocation(line: 58, column: 25, scope: !15)
!115 = !DILocation(line: 60, column: 43, scope: !15)
!116 = !DILocalVariable(name: "idx", scope: !15, file: !5, line: 60, type: !117)
!117 = !DIBasicType(name: "long", size: 64, encoding: DW_ATE_signed)
!118 = !DILocation(line: 60, column: 13, scope: !15)
!119 = !DILocalVariable(name: "item", scope: !15, file: !5, line: 60, type: !32)
!120 = !DILocation(line: 60, column: 23, scope: !15)
!121 = !DILocation(line: 61, column: 9, scope: !15)
!122 = !DILocation(line: 63, column: 19, scope: !15)
!123 = !DILocation(line: 63, column: 25, scope: !15)
!124 = !DILocation(line: 64, column: 19, scope: !15)
!125 = !DILocation(line: 64, column: 25, scope: !15)
!126 = !DILocation(line: 65, column: 19, scope: !15)
!127 = !DILocation(line: 65, column: 25, scope: !15)
!128 = !DILocation(line: 67, column: 5, scope: !15)
