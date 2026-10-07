; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.IntPair = type { i32, i32 }
%struct.IntTriple = type { i32, i32, i32 }
%struct.ByteTriple = type { i8, i8, i8 }
%struct.BoolPair = type { i1, i1 }
%struct.DoublePair = type { double, double }
%struct.SingleDouble = type { double }
%struct.DoubleLong = type { double, i64 }
%struct.IntLong = type { i32, i64 }
%struct.LongByte = type { i64, i8 }
%struct.IntArray = type { [3 x i32] }
%struct.Large = type { i64, i64, i64 }
%struct.Packed = type <{ i8, i64 }>
%struct.NonTrivial = type { i32 }

@anon.array.0 = private unnamed_addr constant [3 x i32] [i32 14, i32 15, i32 16]
@anon.string.0 = private unnamed_addr constant [84 x i8] c"Assertion failed: Condition 'intPair.a == 1 && intPair.b == 2' evaluated to false.\0A\00", align 4
@anon.string.1 = private unnamed_addr constant [108 x i8] c"Assertion failed: Condition 'intTriple.a == 3 && intTriple.b == 4 && intTriple.c == 5' evaluated to false.\0A\00", align 4
@anon.string.2 = private unnamed_addr constant [147 x i8] c"Assertion failed: Condition 'byteTriple.a == cast<byte>(6) && byteTriple.b == cast<byte>(7) && byteTriple.c == cast<byte>(8)' evaluated to false.\0A\00", align 4
@anon.string.3 = private unnamed_addr constant [77 x i8] c"Assertion failed: Condition 'boolPair.a && !boolPair.b' evaluated to false.\0A\00", align 4
@anon.string.4 = private unnamed_addr constant [94 x i8] c"Assertion failed: Condition 'doublePair.x == 1.5 && doublePair.y == 2.5' evaluated to false.\0A\00", align 4
@anon.string.5 = private unnamed_addr constant [73 x i8] c"Assertion failed: Condition 'singleDouble.x == 3.5' evaluated to false.\0A\00", align 4
@anon.string.6 = private unnamed_addr constant [93 x i8] c"Assertion failed: Condition 'doubleLong.x == 4.5 && doubleLong.n == 9l' evaluated to false.\0A\00", align 4
@anon.string.7 = private unnamed_addr constant [87 x i8] c"Assertion failed: Condition 'intLong.a == 10 && intLong.b == 11l' evaluated to false.\0A\00", align 4
@anon.string.8 = private unnamed_addr constant [101 x i8] c"Assertion failed: Condition 'longByte.a == 12l && longByte.b == cast<byte>(13)' evaluated to false.\0A\00", align 4
@anon.string.9 = private unnamed_addr constant [132 x i8] c"Assertion failed: Condition 'intArray.values[0] == 14 && intArray.values[1] == 15 && intArray.values[2] == 16' evaluated to false.\0A\00", align 4
@anon.string.10 = private unnamed_addr constant [102 x i8] c"Assertion failed: Condition 'large.a == 17l && large.b == 18l && large.c == 19l' evaluated to false.\0A\00", align 4
@anon.string.11 = private unnamed_addr constant [97 x i8] c"Assertion failed: Condition 'packed.a == cast<byte>(20) && packed.b == 21l' evaluated to false.\0A\00", align 4
@anon.string.12 = private unnamed_addr constant [74 x i8] c"Assertion failed: Condition 'nonTrivial.value == 22' evaluated to false.\0A\00", align 4
@printf.str.0 = private unnamed_addr constant [24 x i8] c"All assertions passed!\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN10NonTrivial4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i64 @_Z11makeIntPairv() #0 {
  %1 = alloca %struct.IntPair, align 8
  store %struct.IntPair { i32 1, i32 2 }, ptr %1, align 4
  %2 = load i64, ptr %1, align 8
  ret i64 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z13makeIntTriplev(ptr dead_on_unwind noalias writable sret(%struct.IntTriple) align 4 %0) #0 {
  store %struct.IntTriple { i32 3, i32 4, i32 5 }, ptr %0, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z14makeByteTriplev(ptr dead_on_unwind noalias writable sret(%struct.ByteTriple) align 1 %0) #0 {
  %2 = alloca %struct.ByteTriple, align 8
  store i8 6, ptr %2, align 1
  %3 = getelementptr inbounds nuw %struct.ByteTriple, ptr %2, i32 0, i32 1
  store i8 7, ptr %3, align 1
  %4 = getelementptr inbounds nuw %struct.ByteTriple, ptr %2, i32 0, i32 2
  store i8 8, ptr %4, align 1
  %5 = load %struct.ByteTriple, ptr %2, align 1
  store %struct.ByteTriple %5, ptr %0, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i16 @_Z12makeBoolPairv() #0 {
  %1 = alloca %struct.BoolPair, align 8
  store %struct.BoolPair { i1 true, i1 false }, ptr %1, align 1
  %2 = load i16, ptr %1, align 2
  ret i16 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z14makeDoublePairv(ptr dead_on_unwind noalias writable sret(%struct.DoublePair) align 8 %0) #0 {
  store %struct.DoublePair { double 1.500000e+00, double 2.500000e+00 }, ptr %0, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal i64 @_Z16makeSingleDoublev() #0 {
  %1 = alloca %struct.SingleDouble, align 8
  store %struct.SingleDouble { double 3.500000e+00 }, ptr %1, align 8
  %2 = load i64, ptr %1, align 8
  ret i64 %2
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z14makeDoubleLongv(ptr dead_on_unwind noalias writable sret(%struct.DoubleLong) align 8 %0) #0 {
  store %struct.DoubleLong { double 4.500000e+00, i64 9 }, ptr %0, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z11makeIntLongv(ptr dead_on_unwind noalias writable sret(%struct.IntLong) align 8 %0) #0 {
  store %struct.IntLong { i32 10, i64 11 }, ptr %0, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z12makeLongBytev(ptr dead_on_unwind noalias writable sret(%struct.LongByte) align 8 %0) #0 {
  %2 = alloca %struct.LongByte, align 8
  store i64 12, ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.LongByte, ptr %2, i32 0, i32 1
  store i8 13, ptr %3, align 1
  %4 = load %struct.LongByte, ptr %2, align 8
  store %struct.LongByte %4, ptr %0, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z12makeIntArrayv(ptr dead_on_unwind noalias writable sret(%struct.IntArray) align 4 %0) #0 {
  store %struct.IntArray { [3 x i32] [i32 14, i32 15, i32 16] }, ptr %0, align 4
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z9makeLargev(ptr dead_on_unwind noalias writable sret(%struct.Large) align 8 %0) #0 {
  store %struct.Large { i64 17, i64 18, i64 19 }, ptr %0, align 8
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z10makePackedv(ptr dead_on_unwind noalias writable sret(%struct.Packed) align 1 %0) #0 {
  %2 = alloca %struct.Packed, align 8
  store i8 20, ptr %2, align 1
  %3 = getelementptr inbounds nuw %struct.Packed, ptr %2, i32 0, i32 1
  store i64 21, ptr %3, align 8
  %4 = load %struct.Packed, ptr %2, align 1
  store %struct.Packed %4, ptr %0, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_Z14makeNonTrivialv(ptr dead_on_unwind noalias writable sret(%struct.NonTrivial) align 4 %0) #0 {
  store %struct.NonTrivial { i32 22 }, ptr %0, align 4
  ret void
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %intPair = alloca %struct.IntPair, align 8
  %intTriple = alloca %struct.IntTriple, align 8
  %byteTriple = alloca %struct.ByteTriple, align 8
  %boolPair = alloca %struct.BoolPair, align 8
  %doublePair = alloca %struct.DoublePair, align 8
  %singleDouble = alloca %struct.SingleDouble, align 8
  %doubleLong = alloca %struct.DoubleLong, align 8
  %intLong = alloca %struct.IntLong, align 8
  %longByte = alloca %struct.LongByte, align 8
  %intArray = alloca %struct.IntArray, align 8
  %large = alloca %struct.Large, align 8
  %packed = alloca %struct.Packed, align 8
  %nonTrivial = alloca %struct.NonTrivial, align 8
  %1 = call i64 @_Z11makeIntPairv()
  store i64 %1, ptr %intPair, align 8
  %a.addr = getelementptr inbounds %struct.IntPair, ptr %intPair, i64 0, i32 0
  %2 = load i32, ptr %a.addr, align 4
  %3 = icmp eq i32 %2, 1
  br i1 %3, label %land.1.L129C12, label %land.exit.L129C12

land.1.L129C12:                                   ; preds = %0
  %b.addr = getelementptr inbounds %struct.IntPair, ptr %intPair, i64 0, i32 1
  %4 = load i32, ptr %b.addr, align 4
  %5 = icmp eq i32 %4, 2
  br label %land.exit.L129C12

land.exit.L129C12:                                ; preds = %land.1.L129C12, %0
  %land_phi = phi i1 [ %3, %0 ], [ %5, %land.1.L129C12 ]
  br i1 %land_phi, label %assert.exit.L129, label %assert.then.L129, !prof !5

assert.then.L129:                                 ; preds = %land.exit.L129C12
  %6 = call ptr @__acrt_iob_func(i32 2)
  %7 = call i32 (ptr, ptr, ...) @fprintf(ptr %6, ptr @anon.string.0)
  call void @exit(i32 1)
  unreachable

assert.exit.L129:                                 ; preds = %land.exit.L129C12
  call void @_Z13makeIntTriplev(ptr dead_on_unwind writable sret(%struct.IntTriple) align 4 %intTriple)
  %a.addr1 = getelementptr inbounds %struct.IntTriple, ptr %intTriple, i64 0, i32 0
  %8 = load i32, ptr %a.addr1, align 4
  %9 = icmp eq i32 %8, 3
  br i1 %9, label %land.1.L131C12, label %land.exit.L131C12

land.1.L131C12:                                   ; preds = %assert.exit.L129
  %b.addr2 = getelementptr inbounds %struct.IntTriple, ptr %intTriple, i64 0, i32 1
  %10 = load i32, ptr %b.addr2, align 4
  %11 = icmp eq i32 %10, 4
  br i1 %11, label %land.2.L131C12, label %land.exit.L131C12

land.2.L131C12:                                   ; preds = %land.1.L131C12
  %c.addr = getelementptr inbounds %struct.IntTriple, ptr %intTriple, i64 0, i32 2
  %12 = load i32, ptr %c.addr, align 4
  %13 = icmp eq i32 %12, 5
  br label %land.exit.L131C12

land.exit.L131C12:                                ; preds = %land.2.L131C12, %land.1.L131C12, %assert.exit.L129
  %land_phi3 = phi i1 [ %9, %assert.exit.L129 ], [ %11, %land.1.L131C12 ], [ %13, %land.2.L131C12 ]
  br i1 %land_phi3, label %assert.exit.L131, label %assert.then.L131, !prof !5

assert.then.L131:                                 ; preds = %land.exit.L131C12
  %14 = call ptr @__acrt_iob_func(i32 2)
  %15 = call i32 (ptr, ptr, ...) @fprintf(ptr %14, ptr @anon.string.1)
  call void @exit(i32 1)
  unreachable

assert.exit.L131:                                 ; preds = %land.exit.L131C12
  call void @_Z14makeByteTriplev(ptr dead_on_unwind writable sret(%struct.ByteTriple) align 1 %byteTriple)
  %a.addr4 = getelementptr inbounds %struct.ByteTriple, ptr %byteTriple, i64 0, i32 0
  %16 = load i8, ptr %a.addr4, align 1
  %17 = icmp eq i8 %16, 6
  br i1 %17, label %land.1.L133C12, label %land.exit.L133C12

land.1.L133C12:                                   ; preds = %assert.exit.L131
  %b.addr5 = getelementptr inbounds %struct.ByteTriple, ptr %byteTriple, i64 0, i32 1
  %18 = load i8, ptr %b.addr5, align 1
  %19 = icmp eq i8 %18, 7
  br i1 %19, label %land.2.L133C12, label %land.exit.L133C12

land.2.L133C12:                                   ; preds = %land.1.L133C12
  %c.addr6 = getelementptr inbounds %struct.ByteTriple, ptr %byteTriple, i64 0, i32 2
  %20 = load i8, ptr %c.addr6, align 1
  %21 = icmp eq i8 %20, 8
  br label %land.exit.L133C12

land.exit.L133C12:                                ; preds = %land.2.L133C12, %land.1.L133C12, %assert.exit.L131
  %land_phi7 = phi i1 [ %17, %assert.exit.L131 ], [ %19, %land.1.L133C12 ], [ %21, %land.2.L133C12 ]
  br i1 %land_phi7, label %assert.exit.L133, label %assert.then.L133, !prof !5

assert.then.L133:                                 ; preds = %land.exit.L133C12
  %22 = call ptr @__acrt_iob_func(i32 2)
  %23 = call i32 (ptr, ptr, ...) @fprintf(ptr %22, ptr @anon.string.2)
  call void @exit(i32 1)
  unreachable

assert.exit.L133:                                 ; preds = %land.exit.L133C12
  %24 = call i16 @_Z12makeBoolPairv()
  store i16 %24, ptr %boolPair, align 2
  %a.addr8 = getelementptr inbounds %struct.BoolPair, ptr %boolPair, i64 0, i32 0
  %25 = load i1, ptr %a.addr8, align 1
  br i1 %25, label %land.1.L135C12, label %land.exit.L135C12

land.1.L135C12:                                   ; preds = %assert.exit.L133
  %b.addr9 = getelementptr inbounds %struct.BoolPair, ptr %boolPair, i64 0, i32 1
  %26 = load i1, ptr %b.addr9, align 1
  %27 = xor i1 %26, true
  br label %land.exit.L135C12

land.exit.L135C12:                                ; preds = %land.1.L135C12, %assert.exit.L133
  %land_phi10 = phi i1 [ %25, %assert.exit.L133 ], [ %27, %land.1.L135C12 ]
  br i1 %land_phi10, label %assert.exit.L135, label %assert.then.L135, !prof !5

assert.then.L135:                                 ; preds = %land.exit.L135C12
  %28 = call ptr @__acrt_iob_func(i32 2)
  %29 = call i32 (ptr, ptr, ...) @fprintf(ptr %28, ptr @anon.string.3)
  call void @exit(i32 1)
  unreachable

assert.exit.L135:                                 ; preds = %land.exit.L135C12
  call void @_Z14makeDoublePairv(ptr dead_on_unwind writable sret(%struct.DoublePair) align 8 %doublePair)
  %x.addr = getelementptr inbounds %struct.DoublePair, ptr %doublePair, i64 0, i32 0
  %30 = load double, ptr %x.addr, align 8
  %31 = fcmp oeq double %30, 1.500000e+00
  br i1 %31, label %land.1.L137C12, label %land.exit.L137C12

land.1.L137C12:                                   ; preds = %assert.exit.L135
  %y.addr = getelementptr inbounds %struct.DoublePair, ptr %doublePair, i64 0, i32 1
  %32 = load double, ptr %y.addr, align 8
  %33 = fcmp oeq double %32, 2.500000e+00
  br label %land.exit.L137C12

land.exit.L137C12:                                ; preds = %land.1.L137C12, %assert.exit.L135
  %land_phi11 = phi i1 [ %31, %assert.exit.L135 ], [ %33, %land.1.L137C12 ]
  br i1 %land_phi11, label %assert.exit.L137, label %assert.then.L137, !prof !5

assert.then.L137:                                 ; preds = %land.exit.L137C12
  %34 = call ptr @__acrt_iob_func(i32 2)
  %35 = call i32 (ptr, ptr, ...) @fprintf(ptr %34, ptr @anon.string.4)
  call void @exit(i32 1)
  unreachable

assert.exit.L137:                                 ; preds = %land.exit.L137C12
  %36 = call i64 @_Z16makeSingleDoublev()
  store i64 %36, ptr %singleDouble, align 8
  %x.addr12 = getelementptr inbounds %struct.SingleDouble, ptr %singleDouble, i64 0, i32 0
  %37 = load double, ptr %x.addr12, align 8
  %38 = fcmp oeq double %37, 3.500000e+00
  br i1 %38, label %assert.exit.L139, label %assert.then.L139, !prof !5

assert.then.L139:                                 ; preds = %assert.exit.L137
  %39 = call ptr @__acrt_iob_func(i32 2)
  %40 = call i32 (ptr, ptr, ...) @fprintf(ptr %39, ptr @anon.string.5)
  call void @exit(i32 1)
  unreachable

assert.exit.L139:                                 ; preds = %assert.exit.L137
  call void @_Z14makeDoubleLongv(ptr dead_on_unwind writable sret(%struct.DoubleLong) align 8 %doubleLong)
  %x.addr13 = getelementptr inbounds %struct.DoubleLong, ptr %doubleLong, i64 0, i32 0
  %41 = load double, ptr %x.addr13, align 8
  %42 = fcmp oeq double %41, 4.500000e+00
  br i1 %42, label %land.1.L141C12, label %land.exit.L141C12

land.1.L141C12:                                   ; preds = %assert.exit.L139
  %n.addr = getelementptr inbounds %struct.DoubleLong, ptr %doubleLong, i64 0, i32 1
  %43 = load i64, ptr %n.addr, align 8
  %44 = icmp eq i64 %43, 9
  br label %land.exit.L141C12

land.exit.L141C12:                                ; preds = %land.1.L141C12, %assert.exit.L139
  %land_phi14 = phi i1 [ %42, %assert.exit.L139 ], [ %44, %land.1.L141C12 ]
  br i1 %land_phi14, label %assert.exit.L141, label %assert.then.L141, !prof !5

assert.then.L141:                                 ; preds = %land.exit.L141C12
  %45 = call ptr @__acrt_iob_func(i32 2)
  %46 = call i32 (ptr, ptr, ...) @fprintf(ptr %45, ptr @anon.string.6)
  call void @exit(i32 1)
  unreachable

assert.exit.L141:                                 ; preds = %land.exit.L141C12
  call void @_Z11makeIntLongv(ptr dead_on_unwind writable sret(%struct.IntLong) align 8 %intLong)
  %a.addr15 = getelementptr inbounds %struct.IntLong, ptr %intLong, i64 0, i32 0
  %47 = load i32, ptr %a.addr15, align 4
  %48 = icmp eq i32 %47, 10
  br i1 %48, label %land.1.L143C12, label %land.exit.L143C12

land.1.L143C12:                                   ; preds = %assert.exit.L141
  %b.addr16 = getelementptr inbounds %struct.IntLong, ptr %intLong, i64 0, i32 1
  %49 = load i64, ptr %b.addr16, align 8
  %50 = icmp eq i64 %49, 11
  br label %land.exit.L143C12

land.exit.L143C12:                                ; preds = %land.1.L143C12, %assert.exit.L141
  %land_phi17 = phi i1 [ %48, %assert.exit.L141 ], [ %50, %land.1.L143C12 ]
  br i1 %land_phi17, label %assert.exit.L143, label %assert.then.L143, !prof !5

assert.then.L143:                                 ; preds = %land.exit.L143C12
  %51 = call ptr @__acrt_iob_func(i32 2)
  %52 = call i32 (ptr, ptr, ...) @fprintf(ptr %51, ptr @anon.string.7)
  call void @exit(i32 1)
  unreachable

assert.exit.L143:                                 ; preds = %land.exit.L143C12
  call void @_Z12makeLongBytev(ptr dead_on_unwind writable sret(%struct.LongByte) align 8 %longByte)
  %a.addr18 = getelementptr inbounds %struct.LongByte, ptr %longByte, i64 0, i32 0
  %53 = load i64, ptr %a.addr18, align 8
  %54 = icmp eq i64 %53, 12
  br i1 %54, label %land.1.L145C12, label %land.exit.L145C12

land.1.L145C12:                                   ; preds = %assert.exit.L143
  %b.addr19 = getelementptr inbounds %struct.LongByte, ptr %longByte, i64 0, i32 1
  %55 = load i8, ptr %b.addr19, align 1
  %56 = icmp eq i8 %55, 13
  br label %land.exit.L145C12

land.exit.L145C12:                                ; preds = %land.1.L145C12, %assert.exit.L143
  %land_phi20 = phi i1 [ %54, %assert.exit.L143 ], [ %56, %land.1.L145C12 ]
  br i1 %land_phi20, label %assert.exit.L145, label %assert.then.L145, !prof !5

assert.then.L145:                                 ; preds = %land.exit.L145C12
  %57 = call ptr @__acrt_iob_func(i32 2)
  %58 = call i32 (ptr, ptr, ...) @fprintf(ptr %57, ptr @anon.string.8)
  call void @exit(i32 1)
  unreachable

assert.exit.L145:                                 ; preds = %land.exit.L145C12
  call void @_Z12makeIntArrayv(ptr dead_on_unwind writable sret(%struct.IntArray) align 4 %intArray)
  %values.addr = getelementptr inbounds %struct.IntArray, ptr %intArray, i64 0, i32 0
  %59 = getelementptr inbounds [3 x i32], ptr %values.addr, i64 0, i32 0
  %60 = load i32, ptr %59, align 4
  %61 = icmp eq i32 %60, 14
  br i1 %61, label %land.1.L147C12, label %land.exit.L147C12

land.1.L147C12:                                   ; preds = %assert.exit.L145
  %values.addr21 = getelementptr inbounds %struct.IntArray, ptr %intArray, i64 0, i32 0
  %62 = getelementptr inbounds [3 x i32], ptr %values.addr21, i64 0, i32 1
  %63 = load i32, ptr %62, align 4
  %64 = icmp eq i32 %63, 15
  br i1 %64, label %land.2.L147C12, label %land.exit.L147C12

land.2.L147C12:                                   ; preds = %land.1.L147C12
  %values.addr22 = getelementptr inbounds %struct.IntArray, ptr %intArray, i64 0, i32 0
  %65 = getelementptr inbounds [3 x i32], ptr %values.addr22, i64 0, i32 2
  %66 = load i32, ptr %65, align 4
  %67 = icmp eq i32 %66, 16
  br label %land.exit.L147C12

land.exit.L147C12:                                ; preds = %land.2.L147C12, %land.1.L147C12, %assert.exit.L145
  %land_phi23 = phi i1 [ %61, %assert.exit.L145 ], [ %64, %land.1.L147C12 ], [ %67, %land.2.L147C12 ]
  br i1 %land_phi23, label %assert.exit.L147, label %assert.then.L147, !prof !5

assert.then.L147:                                 ; preds = %land.exit.L147C12
  %68 = call ptr @__acrt_iob_func(i32 2)
  %69 = call i32 (ptr, ptr, ...) @fprintf(ptr %68, ptr @anon.string.9)
  call void @exit(i32 1)
  unreachable

assert.exit.L147:                                 ; preds = %land.exit.L147C12
  call void @_Z9makeLargev(ptr dead_on_unwind writable sret(%struct.Large) align 8 %large)
  %a.addr24 = getelementptr inbounds %struct.Large, ptr %large, i64 0, i32 0
  %70 = load i64, ptr %a.addr24, align 8
  %71 = icmp eq i64 %70, 17
  br i1 %71, label %land.1.L149C12, label %land.exit.L149C12

land.1.L149C12:                                   ; preds = %assert.exit.L147
  %b.addr25 = getelementptr inbounds %struct.Large, ptr %large, i64 0, i32 1
  %72 = load i64, ptr %b.addr25, align 8
  %73 = icmp eq i64 %72, 18
  br i1 %73, label %land.2.L149C12, label %land.exit.L149C12

land.2.L149C12:                                   ; preds = %land.1.L149C12
  %c.addr26 = getelementptr inbounds %struct.Large, ptr %large, i64 0, i32 2
  %74 = load i64, ptr %c.addr26, align 8
  %75 = icmp eq i64 %74, 19
  br label %land.exit.L149C12

land.exit.L149C12:                                ; preds = %land.2.L149C12, %land.1.L149C12, %assert.exit.L147
  %land_phi27 = phi i1 [ %71, %assert.exit.L147 ], [ %73, %land.1.L149C12 ], [ %75, %land.2.L149C12 ]
  br i1 %land_phi27, label %assert.exit.L149, label %assert.then.L149, !prof !5

assert.then.L149:                                 ; preds = %land.exit.L149C12
  %76 = call ptr @__acrt_iob_func(i32 2)
  %77 = call i32 (ptr, ptr, ...) @fprintf(ptr %76, ptr @anon.string.10)
  call void @exit(i32 1)
  unreachable

assert.exit.L149:                                 ; preds = %land.exit.L149C12
  call void @_Z10makePackedv(ptr dead_on_unwind writable sret(%struct.Packed) align 1 %packed)
  %a.addr28 = getelementptr inbounds %struct.Packed, ptr %packed, i64 0, i32 0
  %78 = load i8, ptr %a.addr28, align 1
  %79 = icmp eq i8 %78, 20
  br i1 %79, label %land.1.L151C12, label %land.exit.L151C12

land.1.L151C12:                                   ; preds = %assert.exit.L149
  %b.addr29 = getelementptr inbounds %struct.Packed, ptr %packed, i64 0, i32 1
  %80 = load i64, ptr %b.addr29, align 8
  %81 = icmp eq i64 %80, 21
  br label %land.exit.L151C12

land.exit.L151C12:                                ; preds = %land.1.L151C12, %assert.exit.L149
  %land_phi30 = phi i1 [ %79, %assert.exit.L149 ], [ %81, %land.1.L151C12 ]
  br i1 %land_phi30, label %assert.exit.L151, label %assert.then.L151, !prof !5

assert.then.L151:                                 ; preds = %land.exit.L151C12
  %82 = call ptr @__acrt_iob_func(i32 2)
  %83 = call i32 (ptr, ptr, ...) @fprintf(ptr %82, ptr @anon.string.11)
  call void @exit(i32 1)
  unreachable

assert.exit.L151:                                 ; preds = %land.exit.L151C12
  call void @_Z14makeNonTrivialv(ptr dead_on_unwind writable sret(%struct.NonTrivial) align 4 %nonTrivial)
  %value.addr = getelementptr inbounds %struct.NonTrivial, ptr %nonTrivial, i64 0, i32 0
  %84 = load i32, ptr %value.addr, align 4
  %85 = icmp eq i32 %84, 22
  br i1 %85, label %assert.exit.L153, label %assert.then.L153, !prof !5

assert.then.L153:                                 ; preds = %assert.exit.L151
  %86 = call ptr @__acrt_iob_func(i32 2)
  %87 = call i32 (ptr, ptr, ...) @fprintf(ptr %86, ptr @anon.string.12)
  call void @exit(i32 1)
  unreachable

assert.exit.L153:                                 ; preds = %assert.exit.L151
  %88 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0)
  call void @_ZN10NonTrivial4dtorEv(ptr noundef nonnull align 4 dereferenceable(4) %nonTrivial)
  ret i32 0
}

; Function Attrs: nofree
declare noundef i32 @fprintf(ptr noundef captures(none), ptr noundef readonly captures(none), ...) local_unnamed_addr #2

; Function Attrs: nounwind
declare dso_local noundef ptr @__acrt_iob_func(i32 noundef) #3

; Function Attrs: cold noreturn nounwind
declare void @exit(i32) #4

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #5

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree }
attributes #3 = { nounwind }
attributes #4 = { cold noreturn nounwind }
attributes #5 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
!5 = !{!"branch_weights", i32 1048575, i32 1}
