; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.Vector = type { ptr, i64, i64 }
%struct.VectorIterator = type { ptr, i64 }
%struct.Pair = type { i64, ptr }

@anon.string.0 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.getSize() == 3' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'pair.getFirst() == 2' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 'pair.getSecond() == 9876' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == -99' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\00", align 1
@anon.string.16 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 123' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4321' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9876' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4322' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9877' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'vi.get(0) == 124' evaluated to false.\00", align 1
@anon.string.23 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(1) == 4323' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'vi.get(2) == 9879' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %vi = alloca %struct.Vector, align 8
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  %it = alloca %struct.VectorIterator, align 8
  %pair = alloca %struct.Pair, align 8
  %4 = alloca i32, align 4
  %5 = alloca i32, align 4
  %6 = alloca %struct.VectorIterator, align 8
  %7 = alloca %struct.VectorIterator, align 8
  %8 = alloca %struct.VectorIterator, align 8
  call void @_ZN6VectorIiE4ctorEv(ptr nonnull %vi) #3
  store i32 123, ptr %1, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr nonnull %vi, ptr nonnull %1) #3
  store i32 4321, ptr %2, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr nonnull %vi, ptr nonnull %2) #3
  store i32 9876, ptr %3, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr nonnull %vi, ptr nonnull %3) #3
  %9 = call i64 @_ZN6VectorIiE7getSizeEv(ptr nonnull %vi) #3
  %10 = icmp eq i64 %9, 3
  br i1 %10, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %0
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %0
  %12 = call %struct.VectorIterator @"_Z7iterateR22std/data/vector.VectorIiE"(ptr nonnull %vi) #3
  %.fca.0.extract21 = extractvalue %struct.VectorIterator %12, 0
  %.fca.1.extract23 = extractvalue %struct.VectorIterator %12, 1
  %.fca.1.gep24 = getelementptr inbounds %struct.VectorIterator, ptr %it, i64 0, i32 1
  store ptr %.fca.0.extract21, ptr %it, align 8
  store i64 %.fca.1.extract23, ptr %.fca.1.gep24, align 8
  %13 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %13, label %assert.exit.L16, label %assert.then.L16, !prof !0

assert.then.L16:                                  ; preds = %assert.exit.L12
  %14 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L16:                                  ; preds = %assert.exit.L12
  %15 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %it) #3
  %16 = load i32, ptr %15, align 4
  %17 = icmp eq i32 %16, 123
  br i1 %17, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L16
  %18 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L16
  %19 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %it) #3
  %20 = load i32, ptr %19, align 4
  %21 = icmp eq i32 %20, 123
  br i1 %21, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %22 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_ZN14VectorIteratorIiE4nextEv(ptr nonnull %it) #3
  %23 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %it) #3
  %24 = load i32, ptr %23, align 4
  %25 = icmp eq i32 %24, 4321
  br i1 %25, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L18
  %26 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L18
  %27 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %27, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L20
  %28 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  call void @_ZN14VectorIteratorIiE4nextEv(ptr nonnull %it) #3
  %29 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr nonnull %it) #3
  %.fca.0.extract13 = extractvalue %struct.Pair %29, 0
  %.fca.1.extract15 = extractvalue %struct.Pair %29, 1
  %.fca.1.gep16 = getelementptr inbounds %struct.Pair, ptr %pair, i64 0, i32 1
  store i64 %.fca.0.extract13, ptr %pair, align 8
  store ptr %.fca.1.extract15, ptr %.fca.1.gep16, align 8
  %30 = call ptr @_ZN4PairImRiE8getFirstEv(ptr nonnull %pair) #3
  %31 = load i64, ptr %30, align 8
  %32 = icmp eq i64 %31, 2
  br i1 %32, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L21
  %33 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L21
  %34 = call ptr @_ZN4PairImRiE9getSecondEv(ptr nonnull %pair) #3
  %35 = load i32, ptr %34, align 4
  %36 = icmp eq i32 %35, 9876
  br i1 %36, label %assert.exit.L25, label %assert.then.L25, !prof !0

assert.then.L25:                                  ; preds = %assert.exit.L24
  %37 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L24
  call void @_ZN14VectorIteratorIiE4nextEv(ptr nonnull %it) #3
  %38 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %38, label %assert.then.L27, label %assert.exit.L27, !prof !1

assert.then.L27:                                  ; preds = %assert.exit.L25
  %39 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L27:                                  ; preds = %assert.exit.L25
  store i32 321, ptr %4, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr nonnull %vi, ptr nonnull %4) #3
  store i32 -99, ptr %5, align 4
  call void @_ZN6VectorIiE8pushBackERKi(ptr nonnull %vi, ptr nonnull %5) #3
  %40 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %40, label %assert.exit.L32, label %assert.then.L32, !prof !0

assert.then.L32:                                  ; preds = %assert.exit.L27
  %41 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L27
  call void @_Z13op.minusequalR14VectorIteratorIiEi(ptr nonnull %it, i32 3) #3
  %42 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %it) #3
  %43 = load i32, ptr %42, align 4
  %44 = icmp eq i32 %43, 123
  br i1 %44, label %assert.exit.L36, label %assert.then.L36, !prof !0

assert.then.L36:                                  ; preds = %assert.exit.L32
  %45 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L36:                                  ; preds = %assert.exit.L32
  %46 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %46, label %assert.exit.L37, label %assert.then.L37, !prof !0

assert.then.L37:                                  ; preds = %assert.exit.L36
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L36
  call void @_Z16op.plusplus.postR14VectorIteratorIiE(ptr nonnull %it) #3
  %48 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %it) #3
  %49 = load i32, ptr %48, align 4
  %50 = icmp eq i32 %49, 4321
  br i1 %50, label %assert.exit.L39, label %assert.then.L39, !prof !0

assert.then.L39:                                  ; preds = %assert.exit.L37
  %51 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L37
  call void @_Z18op.minusminus.postR14VectorIteratorIiE(ptr nonnull %it) #3
  %52 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %it) #3
  %53 = load i32, ptr %52, align 4
  %54 = icmp eq i32 %53, 123
  br i1 %54, label %assert.exit.L41, label %assert.then.L41, !prof !0

assert.then.L41:                                  ; preds = %assert.exit.L39
  %55 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1)
  unreachable

assert.exit.L41:                                  ; preds = %assert.exit.L39
  call void @_Z12op.plusequalR14VectorIteratorIiEi(ptr nonnull %it, i32 4) #3
  %56 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %it) #3
  %57 = load i32, ptr %56, align 4
  %58 = icmp eq i32 %57, -99
  br i1 %58, label %assert.exit.L43, label %assert.then.L43, !prof !0

assert.then.L43:                                  ; preds = %assert.exit.L41
  %59 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1)
  unreachable

assert.exit.L43:                                  ; preds = %assert.exit.L41
  call void @_ZN14VectorIteratorIiE4nextEv(ptr nonnull %it) #3
  %60 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %it) #3
  br i1 %60, label %assert.then.L45, label %assert.exit.L45, !prof !1

assert.then.L45:                                  ; preds = %assert.exit.L43
  %61 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L45:                                  ; preds = %assert.exit.L43
  %62 = call %struct.VectorIterator @"_Z7iterateR22std/data/vector.VectorIiE"(ptr nonnull %vi) #3
  %.fca.0.extract9 = extractvalue %struct.VectorIterator %62, 0
  store ptr %.fca.0.extract9, ptr %6, align 8
  %.fca.1.extract11 = extractvalue %struct.VectorIterator %62, 1
  %.fca.1.gep12 = getelementptr inbounds %struct.VectorIterator, ptr %6, i64 0, i32 1
  store i64 %.fca.1.extract11, ptr %.fca.1.gep12, align 8
  %63 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %6) #3
  br i1 %63, label %foreach.body.L48, label %foreach.exit.L48

foreach.body.L48:                                 ; preds = %assert.exit.L45, %foreach.body.L48
  %64 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %6) #3
  call void @_ZN14VectorIteratorIiE4nextEv(ptr nonnull %6) #3
  %65 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %6) #3
  br i1 %65, label %foreach.body.L48, label %foreach.exit.L48

foreach.exit.L48:                                 ; preds = %foreach.body.L48, %assert.exit.L45
  %66 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 0) #3
  %67 = load i32, ptr %66, align 4
  %68 = icmp eq i32 %67, 123
  br i1 %68, label %assert.exit.L51, label %assert.then.L51, !prof !0

assert.then.L51:                                  ; preds = %foreach.exit.L48
  %69 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.16)
  call void @exit(i32 1)
  unreachable

assert.exit.L51:                                  ; preds = %foreach.exit.L48
  %70 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 1) #3
  %71 = load i32, ptr %70, align 4
  %72 = icmp eq i32 %71, 4321
  br i1 %72, label %assert.exit.L52, label %assert.then.L52, !prof !0

assert.then.L52:                                  ; preds = %assert.exit.L51
  %73 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L52:                                  ; preds = %assert.exit.L51
  %74 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 2) #3
  %75 = load i32, ptr %74, align 4
  %76 = icmp eq i32 %75, 9876
  br i1 %76, label %assert.exit.L53, label %assert.then.L53, !prof !0

assert.then.L53:                                  ; preds = %assert.exit.L52
  %77 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.18)
  call void @exit(i32 1)
  unreachable

assert.exit.L53:                                  ; preds = %assert.exit.L52
  %78 = call %struct.VectorIterator @"_Z7iterateR22std/data/vector.VectorIiE"(ptr nonnull %vi) #3
  %.fca.0.extract5 = extractvalue %struct.VectorIterator %78, 0
  store ptr %.fca.0.extract5, ptr %7, align 8
  %.fca.1.extract7 = extractvalue %struct.VectorIterator %78, 1
  %.fca.1.gep8 = getelementptr inbounds %struct.VectorIterator, ptr %7, i64 0, i32 1
  store i64 %.fca.1.extract7, ptr %.fca.1.gep8, align 8
  %79 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %7) #3
  br i1 %79, label %foreach.body.L56, label %foreach.exit.L56

foreach.body.L56:                                 ; preds = %assert.exit.L53, %foreach.body.L56
  %80 = call ptr @_ZN14VectorIteratorIiE3getEv(ptr nonnull %7) #3
  %81 = load i32, ptr %80, align 4
  %82 = add i32 %81, 1
  store i32 %82, ptr %80, align 4
  call void @_ZN14VectorIteratorIiE4nextEv(ptr nonnull %7) #3
  %83 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %7) #3
  br i1 %83, label %foreach.body.L56, label %foreach.exit.L56

foreach.exit.L56:                                 ; preds = %foreach.body.L56, %assert.exit.L53
  %84 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 0) #3
  %85 = load i32, ptr %84, align 4
  %86 = icmp eq i32 %85, 124
  br i1 %86, label %assert.exit.L59, label %assert.then.L59, !prof !0

assert.then.L59:                                  ; preds = %foreach.exit.L56
  %87 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.22)
  call void @exit(i32 1)
  unreachable

assert.exit.L59:                                  ; preds = %foreach.exit.L56
  %88 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 1) #3
  %89 = load i32, ptr %88, align 4
  %90 = icmp eq i32 %89, 4322
  br i1 %90, label %assert.exit.L60, label %assert.then.L60, !prof !0

assert.then.L60:                                  ; preds = %assert.exit.L59
  %91 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.20)
  call void @exit(i32 1)
  unreachable

assert.exit.L60:                                  ; preds = %assert.exit.L59
  %92 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 2) #3
  %93 = load i32, ptr %92, align 4
  %94 = icmp eq i32 %93, 9877
  br i1 %94, label %assert.exit.L61, label %assert.then.L61, !prof !0

assert.then.L61:                                  ; preds = %assert.exit.L60
  %95 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1)
  unreachable

assert.exit.L61:                                  ; preds = %assert.exit.L60
  %96 = call %struct.VectorIterator @"_Z7iterateR22std/data/vector.VectorIiE"(ptr nonnull %vi) #3
  %.fca.0.extract = extractvalue %struct.VectorIterator %96, 0
  store ptr %.fca.0.extract, ptr %8, align 8
  %.fca.1.extract = extractvalue %struct.VectorIterator %96, 1
  %.fca.1.gep = getelementptr inbounds %struct.VectorIterator, ptr %8, i64 0, i32 1
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  %97 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %8) #3
  br i1 %97, label %foreach.body.L63, label %foreach.exit.L63

foreach.body.L63:                                 ; preds = %assert.exit.L61, %foreach.body.L63
  %pair3 = call %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr nonnull %8) #3
  %pair3.fca.0.extract = extractvalue %struct.Pair %pair3, 0
  %pair3.fca.1.extract = extractvalue %struct.Pair %pair3, 1
  %98 = trunc i64 %pair3.fca.0.extract to i32
  %99 = load i32, ptr %pair3.fca.1.extract, align 4
  %100 = add i32 %99, %98
  store i32 %100, ptr %pair3.fca.1.extract, align 4
  call void @_ZN14VectorIteratorIiE4nextEv(ptr nonnull %8) #3
  %101 = call i1 @_ZN14VectorIteratorIiE7isValidEv(ptr nonnull %8) #3
  br i1 %101, label %foreach.body.L63, label %foreach.exit.L63

foreach.exit.L63:                                 ; preds = %foreach.body.L63, %assert.exit.L61
  %102 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 0) #3
  %103 = load i32, ptr %102, align 4
  %104 = icmp eq i32 %103, 124
  br i1 %104, label %assert.exit.L66, label %assert.then.L66, !prof !0

assert.then.L66:                                  ; preds = %foreach.exit.L63
  %105 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.22)
  call void @exit(i32 1)
  unreachable

assert.exit.L66:                                  ; preds = %foreach.exit.L63
  %106 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 1) #3
  %107 = load i32, ptr %106, align 4
  %108 = icmp eq i32 %107, 4323
  br i1 %108, label %assert.exit.L67, label %assert.then.L67, !prof !0

assert.then.L67:                                  ; preds = %assert.exit.L66
  %109 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.23)
  call void @exit(i32 1)
  unreachable

assert.exit.L67:                                  ; preds = %assert.exit.L66
  %110 = call ptr @_ZN6VectorIiE3getEj(ptr nonnull %vi, i32 2) #3
  %111 = load i32, ptr %110, align 4
  %112 = icmp eq i32 %111, 9879
  br i1 %112, label %assert.exit.L68, label %assert.then.L68, !prof !0

assert.then.L68:                                  ; preds = %assert.exit.L67
  %113 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.24)
  call void @exit(i32 1)
  unreachable

assert.exit.L68:                                  ; preds = %assert.exit.L67
  %114 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  call void @_ZN6VectorIiE4dtorEv(ptr nonnull %vi) #3
  ret i32 0
}

declare void @_ZN6VectorIiE4ctorEv(ptr) local_unnamed_addr

declare void @_ZN6VectorIiE8pushBackERKi(ptr, ptr) local_unnamed_addr

declare i64 @_ZN6VectorIiE7getSizeEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

declare %struct.VectorIterator @"_Z7iterateR22std/data/vector.VectorIiE"(ptr) local_unnamed_addr

declare i1 @_ZN14VectorIteratorIiE7isValidEv(ptr) local_unnamed_addr

declare ptr @_ZN14VectorIteratorIiE3getEv(ptr) local_unnamed_addr

declare void @_ZN14VectorIteratorIiE4nextEv(ptr) local_unnamed_addr

declare %struct.Pair @_ZN14VectorIteratorIiE6getIdxEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRiE8getFirstEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRiE9getSecondEv(ptr) local_unnamed_addr

declare void @_Z13op.minusequalR14VectorIteratorIiEi(ptr, i32) local_unnamed_addr

declare void @_Z16op.plusplus.postR14VectorIteratorIiE(ptr) local_unnamed_addr

declare void @_Z18op.minusminus.postR14VectorIteratorIiE(ptr) local_unnamed_addr

declare void @_Z12op.plusequalR14VectorIteratorIiEi(ptr, i32) local_unnamed_addr

declare ptr @_ZN6VectorIiE3getEj(ptr, i32) local_unnamed_addr

declare void @_ZN6VectorIiE4dtorEv(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
