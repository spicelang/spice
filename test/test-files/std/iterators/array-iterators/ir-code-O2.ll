; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.ArrayIterator = type { ptr, i64, i64 }
%struct.Pair = type { i64, ptr }

@anon.string.5 = private unnamed_addr constant [71 x i8] c"Assertion failed: Condition 'pair.getFirst() == 2' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [75 x i8] c"Assertion failed: Condition 'pair.getSecond() == 9876' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'it.isValid()' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'it.get() == 4321' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == 123' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'it.get() == -99' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [64 x i8] c"Assertion failed: Condition '!it.isValid()' evaluated to false.\00", align 1
@anon.string.13 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 'a[0] == 123' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[1] == 4321' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[2] == 9876' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[1] == 4322' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[2] == 9877' evaluated to false.\00", align 1
@anon.string.19 = private unnamed_addr constant [62 x i8] c"Assertion failed: Condition 'a[0] == 124' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[1] == 4323' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [63 x i8] c"Assertion failed: Condition 'a[2] == 9879' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %a = alloca [5 x i32], align 4
  %it = alloca %struct.ArrayIterator, align 8
  %pair = alloca %struct.Pair, align 8
  %1 = alloca %struct.ArrayIterator, align 8
  %2 = alloca %struct.ArrayIterator, align 8
  %3 = alloca %struct.ArrayIterator, align 8
  store i32 123, ptr %a, align 4
  %.fca.1.gep49 = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 1
  store i32 4321, ptr %.fca.1.gep49, align 4
  %.fca.2.gep50 = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 2
  store i32 9876, ptr %.fca.2.gep50, align 4
  %.fca.3.gep = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 3
  store i32 321, ptr %.fca.3.gep, align 4
  %.fca.4.gep = getelementptr inbounds [5 x i32], ptr %a, i64 0, i64 4
  store i32 -99, ptr %.fca.4.gep, align 4
  %4 = call %struct.ArrayIterator @_Z7iteratePim(ptr nonnull %a, i64 5) #2
  %.fca.0.extract24 = extractvalue %struct.ArrayIterator %4, 0
  %.fca.1.extract26 = extractvalue %struct.ArrayIterator %4, 1
  %.fca.1.gep27 = getelementptr inbounds %struct.ArrayIterator, ptr %it, i64 0, i32 1
  %.fca.2.extract28 = extractvalue %struct.ArrayIterator %4, 2
  %.fca.2.gep29 = getelementptr inbounds %struct.ArrayIterator, ptr %it, i64 0, i32 2
  store ptr %.fca.0.extract24, ptr %it, align 8
  store i64 %.fca.1.extract26, ptr %.fca.1.gep27, align 8
  store i64 %.fca.2.extract28, ptr %.fca.2.gep29, align 8
  %5 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %it) #2
  br i1 %5, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %0
  %6 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L9:                                   ; preds = %0
  %7 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #2
  %8 = load i32, ptr %7, align 4
  %9 = icmp eq i32 %8, 123
  br i1 %9, label %assert.exit.L10, label %assert.then.L10, !prof !0

assert.then.L10:                                  ; preds = %assert.exit.L9
  %10 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L9
  %11 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #2
  %12 = load i32, ptr %11, align 4
  %13 = icmp eq i32 %12, 123
  br i1 %13, label %assert.exit.L11, label %assert.then.L11, !prof !0

assert.then.L11:                                  ; preds = %assert.exit.L10
  %14 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L11:                                  ; preds = %assert.exit.L10
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %it) #2
  %15 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #2
  %16 = load i32, ptr %15, align 4
  %17 = icmp eq i32 %16, 4321
  br i1 %17, label %assert.exit.L13, label %assert.then.L13, !prof !0

assert.then.L13:                                  ; preds = %assert.exit.L11
  %18 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L13:                                  ; preds = %assert.exit.L11
  %19 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %it) #2
  br i1 %19, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L13
  %20 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L13
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %it) #2
  %21 = call %struct.Pair @_ZN13ArrayIteratorIiE6getIdxEv(ptr nonnull %it) #2
  %.fca.0.extract16 = extractvalue %struct.Pair %21, 0
  %.fca.1.extract18 = extractvalue %struct.Pair %21, 1
  %.fca.1.gep19 = getelementptr inbounds %struct.Pair, ptr %pair, i64 0, i32 1
  store i64 %.fca.0.extract16, ptr %pair, align 8
  store ptr %.fca.1.extract18, ptr %.fca.1.gep19, align 8
  %22 = call ptr @_ZN4PairImRiE8getFirstEv(ptr nonnull %pair) #2
  %23 = load i64, ptr %22, align 8
  %24 = icmp eq i64 %23, 2
  br i1 %24, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L14
  %25 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L14
  %26 = call ptr @_ZN4PairImRiE9getSecondEv(ptr nonnull %pair) #2
  %27 = load i32, ptr %26, align 4
  %28 = icmp eq i32 %27, 9876
  br i1 %28, label %assert.exit.L18, label %assert.then.L18, !prof !0

assert.then.L18:                                  ; preds = %assert.exit.L17
  %29 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L18:                                  ; preds = %assert.exit.L17
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %it) #2
  call void @_Z13op.minusequalR13ArrayIteratorIiEi(ptr nonnull %it, i32 3) #2
  %30 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #2
  %31 = load i32, ptr %30, align 4
  %32 = icmp eq i32 %31, 123
  br i1 %32, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L18
  %33 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L18
  %34 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %it) #2
  br i1 %34, label %assert.exit.L24, label %assert.then.L24, !prof !0

assert.then.L24:                                  ; preds = %assert.exit.L23
  %35 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L24:                                  ; preds = %assert.exit.L23
  call void @_Z16op.plusplus.postR13ArrayIteratorIiE(ptr nonnull %it) #2
  %36 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #2
  %37 = load i32, ptr %36, align 4
  %38 = icmp eq i32 %37, 4321
  br i1 %38, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L24
  %39 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L24
  call void @_Z18op.minusminus.postR13ArrayIteratorIiE(ptr nonnull %it) #2
  %40 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #2
  %41 = load i32, ptr %40, align 4
  %42 = icmp eq i32 %41, 123
  br i1 %42, label %assert.exit.L28, label %assert.then.L28, !prof !0

assert.then.L28:                                  ; preds = %assert.exit.L26
  %43 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L26
  call void @_Z12op.plusequalR13ArrayIteratorIiEi(ptr nonnull %it, i32 4) #2
  %44 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %it) #2
  %45 = load i32, ptr %44, align 4
  %46 = icmp eq i32 %45, -99
  br i1 %46, label %assert.exit.L30, label %assert.then.L30, !prof !0

assert.then.L30:                                  ; preds = %assert.exit.L28
  %47 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L28
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %it) #2
  %48 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %it) #2
  br i1 %48, label %assert.then.L32, label %assert.exit.L32, !prof !1

assert.then.L32:                                  ; preds = %assert.exit.L30
  %49 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L32:                                  ; preds = %assert.exit.L30
  %50 = call %struct.ArrayIterator @_Z7iteratePim(ptr nonnull %a, i64 5) #2
  %.fca.0.extract10 = extractvalue %struct.ArrayIterator %50, 0
  store ptr %.fca.0.extract10, ptr %1, align 8
  %.fca.1.extract12 = extractvalue %struct.ArrayIterator %50, 1
  %.fca.1.gep13 = getelementptr inbounds %struct.ArrayIterator, ptr %1, i64 0, i32 1
  store i64 %.fca.1.extract12, ptr %.fca.1.gep13, align 8
  %.fca.2.extract14 = extractvalue %struct.ArrayIterator %50, 2
  %.fca.2.gep15 = getelementptr inbounds %struct.ArrayIterator, ptr %1, i64 0, i32 2
  store i64 %.fca.2.extract14, ptr %.fca.2.gep15, align 8
  %51 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %1) #2
  br i1 %51, label %foreach.body.L35, label %foreach.exit.L35

foreach.body.L35:                                 ; preds = %assert.exit.L32, %foreach.body.L35
  %52 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %1) #2
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %1) #2
  %53 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %1) #2
  br i1 %53, label %foreach.body.L35, label %foreach.exit.L35

foreach.exit.L35:                                 ; preds = %foreach.body.L35, %assert.exit.L32
  %54 = load i32, ptr %a, align 4
  %55 = icmp eq i32 %54, 123
  br i1 %55, label %assert.exit.L38, label %assert.then.L38, !prof !0

assert.then.L38:                                  ; preds = %foreach.exit.L35
  %56 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.13)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L38:                                  ; preds = %foreach.exit.L35
  %57 = load i32, ptr %.fca.1.gep49, align 4
  %58 = icmp eq i32 %57, 4321
  br i1 %58, label %assert.exit.L39, label %assert.then.L39, !prof !0

assert.then.L39:                                  ; preds = %assert.exit.L38
  %59 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L38
  %60 = load i32, ptr %.fca.2.gep50, align 4
  %61 = icmp eq i32 %60, 9876
  br i1 %61, label %assert.exit.L40, label %assert.then.L40, !prof !0

assert.then.L40:                                  ; preds = %assert.exit.L39
  %62 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L40:                                  ; preds = %assert.exit.L39
  %63 = call %struct.ArrayIterator @_Z7iteratePim(ptr nonnull %a, i64 5) #2
  %.fca.0.extract4 = extractvalue %struct.ArrayIterator %63, 0
  store ptr %.fca.0.extract4, ptr %2, align 8
  %.fca.1.extract6 = extractvalue %struct.ArrayIterator %63, 1
  %.fca.1.gep7 = getelementptr inbounds %struct.ArrayIterator, ptr %2, i64 0, i32 1
  store i64 %.fca.1.extract6, ptr %.fca.1.gep7, align 8
  %.fca.2.extract8 = extractvalue %struct.ArrayIterator %63, 2
  %.fca.2.gep9 = getelementptr inbounds %struct.ArrayIterator, ptr %2, i64 0, i32 2
  store i64 %.fca.2.extract8, ptr %.fca.2.gep9, align 8
  %64 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %2) #2
  br i1 %64, label %foreach.body.L43, label %foreach.exit.L43

foreach.body.L43:                                 ; preds = %assert.exit.L40, %foreach.body.L43
  %65 = call ptr @_ZN13ArrayIteratorIiE3getEv(ptr nonnull %2) #2
  %66 = load i32, ptr %65, align 4
  %67 = add i32 %66, 1
  store i32 %67, ptr %65, align 4
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %2) #2
  %68 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %2) #2
  br i1 %68, label %foreach.body.L43, label %foreach.exit.L43

foreach.exit.L43:                                 ; preds = %foreach.body.L43, %assert.exit.L40
  %69 = load i32, ptr %a, align 4
  %70 = icmp eq i32 %69, 124
  br i1 %70, label %assert.exit.L46, label %assert.then.L46, !prof !0

assert.then.L46:                                  ; preds = %foreach.exit.L43
  %71 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.19)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L46:                                  ; preds = %foreach.exit.L43
  %72 = load i32, ptr %.fca.1.gep49, align 4
  %73 = icmp eq i32 %72, 4322
  br i1 %73, label %assert.exit.L47, label %assert.then.L47, !prof !0

assert.then.L47:                                  ; preds = %assert.exit.L46
  %74 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L47:                                  ; preds = %assert.exit.L46
  %75 = load i32, ptr %.fca.2.gep50, align 4
  %76 = icmp eq i32 %75, 9877
  br i1 %76, label %assert.exit.L48, label %assert.then.L48, !prof !0

assert.then.L48:                                  ; preds = %assert.exit.L47
  %77 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.18)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L48:                                  ; preds = %assert.exit.L47
  %78 = call %struct.ArrayIterator @_Z7iteratePim(ptr nonnull %a, i64 5) #2
  %.fca.0.extract = extractvalue %struct.ArrayIterator %78, 0
  store ptr %.fca.0.extract, ptr %3, align 8
  %.fca.1.extract = extractvalue %struct.ArrayIterator %78, 1
  %.fca.1.gep = getelementptr inbounds %struct.ArrayIterator, ptr %3, i64 0, i32 1
  store i64 %.fca.1.extract, ptr %.fca.1.gep, align 8
  %.fca.2.extract = extractvalue %struct.ArrayIterator %78, 2
  %.fca.2.gep = getelementptr inbounds %struct.ArrayIterator, ptr %3, i64 0, i32 2
  store i64 %.fca.2.extract, ptr %.fca.2.gep, align 8
  %79 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %3) #2
  br i1 %79, label %foreach.body.L50, label %foreach.exit.L50

foreach.body.L50:                                 ; preds = %assert.exit.L48, %foreach.body.L50
  %pair3 = call %struct.Pair @_ZN13ArrayIteratorIiE6getIdxEv(ptr nonnull %3) #2
  %pair3.fca.0.extract = extractvalue %struct.Pair %pair3, 0
  %pair3.fca.1.extract = extractvalue %struct.Pair %pair3, 1
  %80 = trunc i64 %pair3.fca.0.extract to i32
  %81 = load i32, ptr %pair3.fca.1.extract, align 4
  %82 = add i32 %81, %80
  store i32 %82, ptr %pair3.fca.1.extract, align 4
  call void @_ZN13ArrayIteratorIiE4nextEv(ptr nonnull %3) #2
  %83 = call i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr nonnull %3) #2
  br i1 %83, label %foreach.body.L50, label %foreach.exit.L50

foreach.exit.L50:                                 ; preds = %foreach.body.L50, %assert.exit.L48
  %84 = load i32, ptr %a, align 4
  %85 = icmp eq i32 %84, 124
  br i1 %85, label %assert.exit.L53, label %assert.then.L53, !prof !0

assert.then.L53:                                  ; preds = %foreach.exit.L50
  %86 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.19)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L53:                                  ; preds = %foreach.exit.L50
  %87 = load i32, ptr %.fca.1.gep49, align 4
  %88 = icmp eq i32 %87, 4323
  br i1 %88, label %assert.exit.L54, label %assert.then.L54, !prof !0

assert.then.L54:                                  ; preds = %assert.exit.L53
  %89 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.20)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L54:                                  ; preds = %assert.exit.L53
  %90 = load i32, ptr %.fca.2.gep50, align 4
  %91 = icmp eq i32 %90, 9879
  br i1 %91, label %assert.exit.L55, label %assert.then.L55, !prof !0

assert.then.L55:                                  ; preds = %assert.exit.L54
  %92 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1) #2
  unreachable

assert.exit.L55:                                  ; preds = %assert.exit.L54
  %93 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare %struct.ArrayIterator @_Z7iteratePim(ptr, i64) local_unnamed_addr

declare i1 @_ZN13ArrayIteratorIiE7isValidEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

declare void @exit(i32) local_unnamed_addr

declare ptr @_ZN13ArrayIteratorIiE3getEv(ptr) local_unnamed_addr

declare void @_ZN13ArrayIteratorIiE4nextEv(ptr) local_unnamed_addr

declare %struct.Pair @_ZN13ArrayIteratorIiE6getIdxEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRiE8getFirstEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRiE9getSecondEv(ptr) local_unnamed_addr

declare void @_Z13op.minusequalR13ArrayIteratorIiEi(ptr, i32) local_unnamed_addr

declare void @_Z16op.plusplus.postR13ArrayIteratorIiE(ptr) local_unnamed_addr

declare void @_Z18op.minusminus.postR13ArrayIteratorIiE(ptr) local_unnamed_addr

declare void @_Z12op.plusequalR13ArrayIteratorIiEi(ptr, i32) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
