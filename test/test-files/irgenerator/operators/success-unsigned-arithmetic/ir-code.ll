; ModuleID = 'source.spice'
source_filename = "source.spice"

@printf.str.0 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [4 x i8] c"%d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [4 x i8] c"%u\0A\00", align 4
@printf.str.8 = private unnamed_addr constant [6 x i8] c"%lld\0A\00", align 4
@printf.str.9 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4
@printf.str.10 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4
@printf.str.11 = private unnamed_addr constant [4 x i8] c"%u\0A\00", align 4
@printf.str.12 = private unnamed_addr constant [6 x i8] c"%llu\0A\00", align 4

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z12divTemporarymm(i64 noundef %0, i64 noundef %1) #0 {
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  store i64 %0, ptr %a, align 8
  store i64 %1, ptr %b, align 8
  %3 = load i64, ptr %b, align 8
  %4 = load i64, ptr %a, align 8
  %5 = add i64 %4, %3
  %6 = udiv i64 %5, 2
  ret i64 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z20divVariableByLiteralm(i64 noundef %0) #0 {
  %a = alloca i64, align 8
  store i64 %0, ptr %a, align 8
  %2 = load i64, ptr %a, align 8
  %3 = udiv i64 %2, 2
  ret i64 %3
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z12remTemporarymm(i64 noundef %0, i64 noundef %1) #0 {
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  store i64 %0, ptr %a, align 8
  store i64 %1, ptr %b, align 8
  %3 = load i64, ptr %b, align 8
  %4 = load i64, ptr %a, align 8
  %5 = xor i64 %4, %3
  %6 = urem i64 %5, 10
  ret i64 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z19shiftRightTemporarymm(i64 noundef %0, i64 noundef %1) #0 {
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  store i64 %0, ptr %a, align 8
  store i64 %1, ptr %b, align 8
  %3 = load i64, ptr %b, align 8
  %4 = load i64, ptr %a, align 8
  %5 = xor i64 %4, %3
  %6 = lshr i64 %5, 1
  ret i64 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef zeroext i1 @_Z17lessThanTemporarymm(i64 noundef %0, i64 noundef %1) #0 {
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  store i64 %0, ptr %a, align 8
  store i64 %1, ptr %b, align 8
  %3 = load i64, ptr %b, align 8
  %4 = load i64, ptr %a, align 8
  %5 = add i64 %4, %3
  %6 = icmp ult i64 %5, 1
  ret i1 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z15extendTemporaryjj(i32 noundef %0, i32 noundef %1) #0 {
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  store i32 %1, ptr %b, align 4
  %3 = load i32, ptr %b, align 4
  %4 = load i32, ptr %a, align 4
  %5 = add i32 %4, %3
  %6 = zext i32 %5 to i64
  ret i64 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z11wrappingMulm(i64 noundef %0) #0 {
  %a = alloca i64, align 8
  store i64 %0, ptr %a, align 8
  %2 = load i64, ptr %a, align 8
  %3 = xor i64 %2, 1
  %4 = mul i64 %3, -4658895280553007687
  ret i64 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z10bitwiseNotj(i32 noundef %0) #0 {
  %a = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  %2 = load i32, ptr %a, align 4
  %3 = xor i32 %2, -1
  %4 = lshr i32 %3, 28
  ret i32 %4
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z15mixedRankSignedlj(i64 noundef %0, i32 noundef %1) #0 {
  %a = alloca i64, align 8
  %b = alloca i32, align 4
  store i64 %0, ptr %a, align 8
  store i32 %1, ptr %b, align 4
  %3 = load i32, ptr %b, align 4
  %4 = zext i32 %3 to i64
  %5 = load i64, ptr %a, align 8
  %6 = sdiv i64 %5, %4
  ret i64 %6
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z21mixedSameRankUnsignedlm(i64 noundef %0, i64 noundef %1) #0 {
  %a = alloca i64, align 8
  %b = alloca i64, align 8
  store i64 %0, ptr %a, align 8
  store i64 %1, ptr %b, align 8
  %3 = load i64, ptr %b, align 8
  %4 = load i64, ptr %a, align 8
  %5 = udiv i64 %4, %3
  ret i64 %5
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z11compoundDivm(i64 noundef %0) #0 {
  %a = alloca i64, align 8
  %x = alloca i64, align 8
  store i64 %0, ptr %a, align 8
  %2 = load i64, ptr %a, align 8
  store i64 %2, ptr %x, align 8
  %3 = load i64, ptr %x, align 8
  %4 = udiv i64 %3, 4
  store i64 %4, ptr %x, align 8
  %5 = load i64, ptr %x, align 8
  %6 = urem i64 %5, 1000000007
  store i64 %6, ptr %x, align 8
  %7 = load i64, ptr %x, align 8
  ret i64 %7
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i32 @_Z18compoundMixedWidthj(i32 noundef %0) #0 {
  %a = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 %0, ptr %a, align 4
  %2 = load i32, ptr %a, align 4
  store i32 %2, ptr %x, align 4
  %3 = load i32, ptr %x, align 4
  %4 = udiv i32 %3, 2
  store i32 %4, ptr %x, align 4
  %5 = load i32, ptr %a, align 4
  %6 = add i32 %5, 1
  store i32 %6, ptr %y, align 4
  %7 = load i32, ptr %y, align 4
  %8 = urem i32 %7, 3
  store i32 %8, ptr %y, align 4
  %9 = load i32, ptr %y, align 4
  %10 = load i32, ptr %x, align 4
  %11 = add i32 %10, %9
  ret i32 %11
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef i64 @_Z7hashMixm(i64 noundef %0) #0 {
  %hash = alloca i64, align 8
  store i64 %0, ptr %hash, align 8
  %2 = load i64, ptr %hash, align 8
  %3 = add i64 %2, -7046029254386353131
  store i64 %3, ptr %hash, align 8
  %4 = load i64, ptr %hash, align 8
  %5 = lshr i64 %4, 30
  %6 = load i64, ptr %hash, align 8
  %7 = xor i64 %6, %5
  %8 = mul i64 %7, -4658895280553007687
  store i64 %8, ptr %hash, align 8
  %9 = load i64, ptr %hash, align 8
  %10 = lshr i64 %9, 27
  %11 = load i64, ptr %hash, align 8
  %12 = xor i64 %11, %10
  %13 = mul i64 %12, -7723592293110705685
  store i64 %13, ptr %hash, align 8
  %14 = load i64, ptr %hash, align 8
  %15 = lshr i64 %14, 31
  %16 = load i64, ptr %hash, align 8
  %17 = xor i64 %16, %15
  store i64 %17, ptr %hash, align 8
  %18 = load i64, ptr %hash, align 8
  ret i64 %18
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #1 {
  %big = alloca i64, align 8
  store i64 -9223372036854775808, ptr %big, align 8
  %1 = load i64, ptr %big, align 8
  %2 = call noundef i64 @_Z12divTemporarymm(i64 noundef %1, i64 noundef 2)
  %3 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i64 noundef %2)
  %4 = load i64, ptr %big, align 8
  %5 = call noundef i64 @_Z20divVariableByLiteralm(i64 noundef %4)
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 noundef %5)
  %7 = load i64, ptr %big, align 8
  %8 = call noundef i64 @_Z12remTemporarymm(i64 noundef %7, i64 noundef 0)
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i64 noundef %8)
  %10 = load i64, ptr %big, align 8
  %11 = call noundef i64 @_Z19shiftRightTemporarymm(i64 noundef %10, i64 noundef 0)
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i64 noundef %11)
  %13 = load i64, ptr %big, align 8
  %14 = call noundef zeroext i1 @_Z17lessThanTemporarymm(i64 noundef %13, i64 noundef 0)
  %15 = zext i1 %14 to i32
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %15)
  %17 = call noundef i64 @_Z15extendTemporaryjj(i32 noundef -2147483648, i32 noundef 0)
  %18 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i64 noundef %17)
  %19 = call noundef i64 @_Z11wrappingMulm(i64 noundef -16)
  %20 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i64 noundef %19)
  %21 = call noundef i32 @_Z10bitwiseNotj(i32 noundef 0)
  %22 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i32 noundef %21)
  %23 = call noundef i64 @_Z15mixedRankSignedlj(i64 noundef -10, i32 noundef 3)
  %24 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.8, i64 noundef %23)
  %25 = call noundef i64 @_Z21mixedSameRankUnsignedlm(i64 noundef -1, i64 noundef 2)
  %26 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.9, i64 noundef %25)
  %27 = call noundef i64 @_Z11compoundDivm(i64 noundef -1)
  %28 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.10, i64 noundef %27)
  %29 = call noundef i32 @_Z18compoundMixedWidthj(i32 noundef -16)
  %30 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.11, i32 noundef %29)
  %31 = call noundef i64 @_Z7hashMixm(i64 noundef 1)
  %32 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.12, i64 noundef %31)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #2

attributes #0 = { noinline nounwind optnone uwtable }
attributes #1 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #2 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
