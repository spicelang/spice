; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.NumberIterator = type { i32, i32, i32 }
%struct.Pair = type { i64, ptr }
%struct.NumberIterator.1 = type { i64, i64, i64 }
%struct.Pair.0 = type { i64, ptr }

@anon.string.1 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 1' evaluated to false.\00", align 1
@anon.string.2 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 2' evaluated to false.\00", align 1
@anon.string.5 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 3' evaluated to false.\00", align 1
@anon.string.6 = private unnamed_addr constant [82 x i8] c"Assertion failed: Condition 'idxAndValueInt.getFirst() == 3l' evaluated to false.\00", align 1
@anon.string.7 = private unnamed_addr constant [82 x i8] c"Assertion failed: Condition 'idxAndValueInt.getSecond() == 4' evaluated to false.\00", align 1
@anon.string.8 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 5' evaluated to false.\00", align 1
@anon.string.9 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itInt.get() == 4' evaluated to false.\00", align 1
@anon.string.10 = private unnamed_addr constant [66 x i8] c"Assertion failed: Condition 'itInt.isValid()' evaluated to false.\00", align 1
@anon.string.11 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition 'itInt.get() == 10' evaluated to false.\00", align 1
@anon.string.12 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition '!itInt.isValid()' evaluated to false.\00", align 1
@anon.string.14 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'itLong.get() == 6l' evaluated to false.\00", align 1
@anon.string.15 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'itLong.get() == 7l' evaluated to false.\00", align 1
@anon.string.17 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.get() == 10l' evaluated to false.\00", align 1
@anon.string.18 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'itLong.get() == 8l' evaluated to false.\00", align 1
@anon.string.19 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.get() == 16l' evaluated to false.\00", align 1
@anon.string.20 = private unnamed_addr constant [84 x i8] c"Assertion failed: Condition 'idxAndValueLong.getFirst() == 11l' evaluated to false.\00", align 1
@anon.string.21 = private unnamed_addr constant [85 x i8] c"Assertion failed: Condition 'idxAndValueLong.getSecond() == 17l' evaluated to false.\00", align 1
@anon.string.22 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.get() == 18l' evaluated to false.\00", align 1
@anon.string.23 = private unnamed_addr constant [70 x i8] c"Assertion failed: Condition 'itLong.get() == 17l' evaluated to false.\00", align 1
@anon.string.24 = private unnamed_addr constant [67 x i8] c"Assertion failed: Condition 'itLong.isValid()' evaluated to false.\00", align 1
@anon.string.25 = private unnamed_addr constant [69 x i8] c"Assertion failed: Condition 'itLong.get() == 45' evaluated to false.\00", align 1
@anon.string.26 = private unnamed_addr constant [68 x i8] c"Assertion failed: Condition '!itLong.isValid()' evaluated to false.\00", align 1
@printf.str.0 = private unnamed_addr constant [23 x i8] c"All assertions passed!\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() local_unnamed_addr #0 {
  %itInt = alloca %struct.NumberIterator, align 8
  %idxAndValueInt = alloca %struct.Pair, align 8
  %itLong = alloca %struct.NumberIterator.1, align 8
  %idxAndValueLong = alloca %struct.Pair.0, align 8
  %1 = tail call %struct.NumberIterator @_Z5rangeii(i32 1, i32 10) #3
  %.fca.0.extract44 = extractvalue %struct.NumberIterator %1, 0
  %.fca.1.extract46 = extractvalue %struct.NumberIterator %1, 1
  %.fca.1.gep47 = getelementptr inbounds %struct.NumberIterator, ptr %itInt, i64 0, i32 1
  %.fca.2.extract48 = extractvalue %struct.NumberIterator %1, 2
  %.fca.2.gep49 = getelementptr inbounds %struct.NumberIterator, ptr %itInt, i64 0, i32 2
  store i32 %.fca.0.extract44, ptr %itInt, align 8
  store i32 %.fca.1.extract46, ptr %.fca.1.gep47, align 4
  store i32 %.fca.2.extract48, ptr %.fca.2.gep49, align 8
  %2 = call i1 @_ZN14NumberIteratorIiE7isValidEv(ptr nonnull %itInt) #3
  br i1 %2, label %assert.exit.L9, label %assert.then.L9, !prof !0

assert.then.L9:                                   ; preds = %0
  %3 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L9:                                   ; preds = %0
  %4 = call ptr @_ZN14NumberIteratorIiE3getEv(ptr nonnull %itInt) #3
  %5 = load i32, ptr %4, align 4
  %6 = icmp eq i32 %5, 1
  br i1 %6, label %assert.exit.L10, label %assert.then.L10, !prof !0

assert.then.L10:                                  ; preds = %assert.exit.L9
  %7 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L10:                                  ; preds = %assert.exit.L9
  call void @_ZN14NumberIteratorIiE4nextEv(ptr nonnull %itInt) #3
  %8 = call ptr @_ZN14NumberIteratorIiE3getEv(ptr nonnull %itInt) #3
  %9 = load i32, ptr %8, align 4
  %10 = icmp eq i32 %9, 2
  br i1 %10, label %assert.exit.L12, label %assert.then.L12, !prof !0

assert.then.L12:                                  ; preds = %assert.exit.L10
  %11 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L12:                                  ; preds = %assert.exit.L10
  call void @_Z12op.plusequalR14NumberIteratorIiEi(ptr nonnull %itInt, i32 3) #3
  %12 = call ptr @_ZN14NumberIteratorIiE3getEv(ptr nonnull %itInt) #3
  %13 = load i32, ptr %12, align 4
  %14 = icmp eq i32 %13, 5
  br i1 %14, label %assert.exit.L14, label %assert.then.L14, !prof !0

assert.then.L14:                                  ; preds = %assert.exit.L12
  %15 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L14:                                  ; preds = %assert.exit.L12
  %16 = call i1 @_ZN14NumberIteratorIiE7isValidEv(ptr nonnull %itInt) #3
  br i1 %16, label %assert.exit.L15, label %assert.then.L15, !prof !0

assert.then.L15:                                  ; preds = %assert.exit.L14
  %17 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L15:                                  ; preds = %assert.exit.L14
  call void @_Z13op.minusequalR14NumberIteratorIiEi(ptr nonnull %itInt, i32 2) #3
  %18 = call ptr @_ZN14NumberIteratorIiE3getEv(ptr nonnull %itInt) #3
  %19 = load i32, ptr %18, align 4
  %20 = icmp eq i32 %19, 3
  br i1 %20, label %assert.exit.L17, label %assert.then.L17, !prof !0

assert.then.L17:                                  ; preds = %assert.exit.L15
  %21 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L17:                                  ; preds = %assert.exit.L15
  call void @_ZN14NumberIteratorIiE4nextEv(ptr nonnull %itInt) #3
  %22 = call %struct.Pair @_ZN14NumberIteratorIiE6getIdxEv(ptr nonnull %itInt) #3
  %.fca.0.extract36 = extractvalue %struct.Pair %22, 0
  %.fca.1.extract38 = extractvalue %struct.Pair %22, 1
  %.fca.1.gep39 = getelementptr inbounds %struct.Pair, ptr %idxAndValueInt, i64 0, i32 1
  store i64 %.fca.0.extract36, ptr %idxAndValueInt, align 8
  store ptr %.fca.1.extract38, ptr %.fca.1.gep39, align 8
  %23 = call ptr @_ZN4PairImRiE8getFirstEv(ptr nonnull %idxAndValueInt) #3
  %24 = load i64, ptr %23, align 8
  %25 = icmp eq i64 %24, 3
  br i1 %25, label %assert.exit.L20, label %assert.then.L20, !prof !0

assert.then.L20:                                  ; preds = %assert.exit.L17
  %26 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L20:                                  ; preds = %assert.exit.L17
  %27 = call ptr @_ZN4PairImRiE9getSecondEv(ptr nonnull %idxAndValueInt) #3
  %28 = load i32, ptr %27, align 4
  %29 = icmp eq i32 %28, 4
  br i1 %29, label %assert.exit.L21, label %assert.then.L21, !prof !0

assert.then.L21:                                  ; preds = %assert.exit.L20
  %30 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L21:                                  ; preds = %assert.exit.L20
  call void @_Z16op.plusplus.postR14NumberIteratorIiE(ptr nonnull %itInt) #3
  %31 = call ptr @_ZN14NumberIteratorIiE3getEv(ptr nonnull %itInt) #3
  %32 = load i32, ptr %31, align 4
  %33 = icmp eq i32 %32, 5
  br i1 %33, label %assert.exit.L23, label %assert.then.L23, !prof !0

assert.then.L23:                                  ; preds = %assert.exit.L21
  %34 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L23:                                  ; preds = %assert.exit.L21
  call void @_Z18op.minusminus.postR14NumberIteratorIiE(ptr nonnull %itInt) #3
  %35 = call ptr @_ZN14NumberIteratorIiE3getEv(ptr nonnull %itInt) #3
  %36 = load i32, ptr %35, align 4
  %37 = icmp eq i32 %36, 4
  br i1 %37, label %assert.exit.L25, label %assert.then.L25, !prof !0

assert.then.L25:                                  ; preds = %assert.exit.L23
  %38 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L25:                                  ; preds = %assert.exit.L23
  %39 = call i1 @_ZN14NumberIteratorIiE7isValidEv(ptr nonnull %itInt) #3
  br i1 %39, label %assert.exit.L26, label %assert.then.L26, !prof !0

assert.then.L26:                                  ; preds = %assert.exit.L25
  %40 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L26:                                  ; preds = %assert.exit.L25
  call void @_Z12op.plusequalR14NumberIteratorIiEi(ptr nonnull %itInt, i32 6) #3
  %41 = call ptr @_ZN14NumberIteratorIiE3getEv(ptr nonnull %itInt) #3
  %42 = load i32, ptr %41, align 4
  %43 = icmp eq i32 %42, 10
  br i1 %43, label %assert.exit.L28, label %assert.then.L28, !prof !0

assert.then.L28:                                  ; preds = %assert.exit.L26
  %44 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L28:                                  ; preds = %assert.exit.L26
  call void @_Z16op.plusplus.postR14NumberIteratorIiE(ptr nonnull %itInt) #3
  %45 = call i1 @_ZN14NumberIteratorIiE7isValidEv(ptr nonnull %itInt) #3
  br i1 %45, label %assert.then.L30, label %assert.exit.L30, !prof !1

assert.then.L30:                                  ; preds = %assert.exit.L28
  %46 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L30:                                  ; preds = %assert.exit.L28
  %47 = call %struct.NumberIterator.1 @_Z5rangell(i64 6, i64 45) #3
  %.fca.0.extract5 = extractvalue %struct.NumberIterator.1 %47, 0
  %.fca.1.extract7 = extractvalue %struct.NumberIterator.1 %47, 1
  %.fca.1.gep8 = getelementptr inbounds %struct.NumberIterator.1, ptr %itLong, i64 0, i32 1
  %.fca.2.extract = extractvalue %struct.NumberIterator.1 %47, 2
  %.fca.2.gep = getelementptr inbounds %struct.NumberIterator.1, ptr %itLong, i64 0, i32 2
  store i64 %.fca.0.extract5, ptr %itLong, align 8
  store i64 %.fca.1.extract7, ptr %.fca.1.gep8, align 8
  store i64 %.fca.2.extract, ptr %.fca.2.gep, align 8
  %48 = call i1 @_ZN14NumberIteratorIlE7isValidEv(ptr nonnull %itLong) #3
  br i1 %48, label %assert.exit.L34, label %assert.then.L34, !prof !0

assert.then.L34:                                  ; preds = %assert.exit.L30
  %49 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.24)
  call void @exit(i32 1)
  unreachable

assert.exit.L34:                                  ; preds = %assert.exit.L30
  %50 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %51 = load i64, ptr %50, align 8
  %52 = icmp eq i64 %51, 6
  br i1 %52, label %assert.exit.L35, label %assert.then.L35, !prof !0

assert.then.L35:                                  ; preds = %assert.exit.L34
  %53 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.14)
  call void @exit(i32 1)
  unreachable

assert.exit.L35:                                  ; preds = %assert.exit.L34
  call void @_ZN14NumberIteratorIlE4nextEv(ptr nonnull %itLong) #3
  %54 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %55 = load i64, ptr %54, align 8
  %56 = icmp eq i64 %55, 7
  br i1 %56, label %assert.exit.L37, label %assert.then.L37, !prof !0

assert.then.L37:                                  ; preds = %assert.exit.L35
  %57 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.15)
  call void @exit(i32 1)
  unreachable

assert.exit.L37:                                  ; preds = %assert.exit.L35
  call void @_Z12op.plusequalR14NumberIteratorIlEl(ptr nonnull %itLong, i64 3) #3
  %58 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %59 = load i64, ptr %58, align 8
  %60 = icmp eq i64 %59, 10
  br i1 %60, label %assert.exit.L39, label %assert.then.L39, !prof !0

assert.then.L39:                                  ; preds = %assert.exit.L37
  %61 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L39:                                  ; preds = %assert.exit.L37
  %62 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %63 = load i64, ptr %62, align 8
  %64 = icmp eq i64 %63, 10
  br i1 %64, label %assert.exit.L40, label %assert.then.L40, !prof !0

assert.then.L40:                                  ; preds = %assert.exit.L39
  %65 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.17)
  call void @exit(i32 1)
  unreachable

assert.exit.L40:                                  ; preds = %assert.exit.L39
  call void @_Z13op.minusequalR14NumberIteratorIlEl(ptr nonnull %itLong, i64 2) #3
  %66 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %67 = load i64, ptr %66, align 8
  %68 = icmp eq i64 %67, 8
  br i1 %68, label %assert.exit.L42, label %assert.then.L42, !prof !0

assert.then.L42:                                  ; preds = %assert.exit.L40
  %69 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.18)
  call void @exit(i32 1)
  unreachable

assert.exit.L42:                                  ; preds = %assert.exit.L40
  call void @_Z12op.plusequalR14NumberIteratorIlEl(ptr nonnull %itLong, i64 8) #3
  %70 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %71 = load i64, ptr %70, align 8
  %72 = icmp eq i64 %71, 16
  br i1 %72, label %assert.exit.L44, label %assert.then.L44, !prof !0

assert.then.L44:                                  ; preds = %assert.exit.L42
  %73 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.19)
  call void @exit(i32 1)
  unreachable

assert.exit.L44:                                  ; preds = %assert.exit.L42
  call void @_ZN14NumberIteratorIlE4nextEv(ptr nonnull %itLong) #3
  %74 = call %struct.Pair.0 @_ZN14NumberIteratorIlE6getIdxEv(ptr nonnull %itLong) #3
  %.fca.0.extract = extractvalue %struct.Pair.0 %74, 0
  %.fca.1.extract = extractvalue %struct.Pair.0 %74, 1
  %.fca.1.gep = getelementptr inbounds %struct.Pair.0, ptr %idxAndValueLong, i64 0, i32 1
  store i64 %.fca.0.extract, ptr %idxAndValueLong, align 8
  store ptr %.fca.1.extract, ptr %.fca.1.gep, align 8
  %75 = call ptr @_ZN4PairImRlE8getFirstEv(ptr nonnull %idxAndValueLong) #3
  %76 = load i64, ptr %75, align 8
  %77 = icmp eq i64 %76, 11
  br i1 %77, label %assert.exit.L47, label %assert.then.L47, !prof !0

assert.then.L47:                                  ; preds = %assert.exit.L44
  %78 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.20)
  call void @exit(i32 1)
  unreachable

assert.exit.L47:                                  ; preds = %assert.exit.L44
  %79 = call ptr @_ZN4PairImRlE9getSecondEv(ptr nonnull %idxAndValueLong) #3
  %80 = load i64, ptr %79, align 8
  %81 = icmp eq i64 %80, 17
  br i1 %81, label %assert.exit.L48, label %assert.then.L48, !prof !0

assert.then.L48:                                  ; preds = %assert.exit.L47
  %82 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.21)
  call void @exit(i32 1)
  unreachable

assert.exit.L48:                                  ; preds = %assert.exit.L47
  call void @_Z16op.plusplus.postR14NumberIteratorIlE(ptr nonnull %itLong) #3
  %83 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %84 = load i64, ptr %83, align 8
  %85 = icmp eq i64 %84, 18
  br i1 %85, label %assert.exit.L50, label %assert.then.L50, !prof !0

assert.then.L50:                                  ; preds = %assert.exit.L48
  %86 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.22)
  call void @exit(i32 1)
  unreachable

assert.exit.L50:                                  ; preds = %assert.exit.L48
  call void @_Z18op.minusminus.postR14NumberIteratorIlE(ptr nonnull %itLong) #3
  %87 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %88 = load i64, ptr %87, align 8
  %89 = icmp eq i64 %88, 17
  br i1 %89, label %assert.exit.L52, label %assert.then.L52, !prof !0

assert.then.L52:                                  ; preds = %assert.exit.L50
  %90 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.23)
  call void @exit(i32 1)
  unreachable

assert.exit.L52:                                  ; preds = %assert.exit.L50
  %91 = call i1 @_ZN14NumberIteratorIlE7isValidEv(ptr nonnull %itLong) #3
  br i1 %91, label %assert.exit.L53, label %assert.then.L53, !prof !0

assert.then.L53:                                  ; preds = %assert.exit.L52
  %92 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.24)
  call void @exit(i32 1)
  unreachable

assert.exit.L53:                                  ; preds = %assert.exit.L52
  call void @_Z12op.plusequalR14NumberIteratorIlEl(ptr nonnull %itLong, i64 28) #3
  %93 = call ptr @_ZN14NumberIteratorIlE3getEv(ptr nonnull %itLong) #3
  %94 = load i64, ptr %93, align 8
  %95 = icmp eq i64 %94, 45
  br i1 %95, label %assert.exit.L55, label %assert.then.L55, !prof !0

assert.then.L55:                                  ; preds = %assert.exit.L53
  %96 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.25)
  call void @exit(i32 1)
  unreachable

assert.exit.L55:                                  ; preds = %assert.exit.L53
  call void @_Z16op.plusplus.postR14NumberIteratorIlE(ptr nonnull %itLong) #3
  %97 = call i1 @_ZN14NumberIteratorIlE7isValidEv(ptr nonnull %itLong) #3
  br i1 %97, label %assert.then.L57, label %assert.exit.L57, !prof !1

assert.then.L57:                                  ; preds = %assert.exit.L55
  %98 = call i32 (ptr, ...) @printf(ptr nonnull dereferenceable(1) @anon.string.26)
  call void @exit(i32 1)
  unreachable

assert.exit.L57:                                  ; preds = %assert.exit.L55
  %99 = call i32 (ptr, ...) @printf(ptr noundef nonnull dereferenceable(1) @printf.str.0)
  ret i32 0
}

declare %struct.NumberIterator @_Z5rangeii(i32, i32) local_unnamed_addr

declare i1 @_ZN14NumberIteratorIiE7isValidEv(ptr) local_unnamed_addr

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) local_unnamed_addr #1

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) local_unnamed_addr #2

declare ptr @_ZN14NumberIteratorIiE3getEv(ptr) local_unnamed_addr

declare void @_ZN14NumberIteratorIiE4nextEv(ptr) local_unnamed_addr

declare void @_Z12op.plusequalR14NumberIteratorIiEi(ptr, i32) local_unnamed_addr

declare void @_Z13op.minusequalR14NumberIteratorIiEi(ptr, i32) local_unnamed_addr

declare %struct.Pair @_ZN14NumberIteratorIiE6getIdxEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRiE8getFirstEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRiE9getSecondEv(ptr) local_unnamed_addr

declare void @_Z16op.plusplus.postR14NumberIteratorIiE(ptr) local_unnamed_addr

declare void @_Z18op.minusminus.postR14NumberIteratorIiE(ptr) local_unnamed_addr

declare %struct.NumberIterator.1 @_Z5rangell(i64, i64) local_unnamed_addr

declare i1 @_ZN14NumberIteratorIlE7isValidEv(ptr) local_unnamed_addr

declare ptr @_ZN14NumberIteratorIlE3getEv(ptr) local_unnamed_addr

declare void @_ZN14NumberIteratorIlE4nextEv(ptr) local_unnamed_addr

declare void @_Z12op.plusequalR14NumberIteratorIlEl(ptr, i64) local_unnamed_addr

declare void @_Z13op.minusequalR14NumberIteratorIlEl(ptr, i64) local_unnamed_addr

declare %struct.Pair.0 @_ZN14NumberIteratorIlE6getIdxEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRlE8getFirstEv(ptr) local_unnamed_addr

declare ptr @_ZN4PairImRlE9getSecondEv(ptr) local_unnamed_addr

declare void @_Z16op.plusplus.postR14NumberIteratorIlE(ptr) local_unnamed_addr

declare void @_Z18op.minusminus.postR14NumberIteratorIlE(ptr) local_unnamed_addr

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { nofree nounwind }
attributes #2 = { cold noreturn nounwind }
attributes #3 = { nounwind }

!0 = !{!"branch_weights", i32 2000, i32 1}
!1 = !{!"branch_weights", i32 1, i32 2000}
