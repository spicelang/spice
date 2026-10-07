; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Inner = type { ptr, i32, i64, ptr }
%struct.Outer = type { %struct.Inner, i16, %struct.Inner }

@_ZTS5Inner = private constant [7 x i8] c"5Inner\00", align 4
@_ZTV8TypeInfo = external global ptr
@_ZTI5Inner = private constant { ptr, ptr } { ptr getelementptr inbounds (ptr, ptr @_ZTV8TypeInfo, i64 2), ptr @_ZTS5Inner }, align 8
@_ZTV5Inner = private unnamed_addr constant { [2 x ptr] } { [2 x ptr] [ptr null, ptr @_ZTI5Inner] }, align 8
@printf.str.0 = private unnamed_addr constant [19 x i8] c"Inner: a=%d, b=%d\0A\00", align 4
@printf.str.1 = private unnamed_addr constant [36 x i8] c"Offsets: a=%d, b=%d, h=%d, size=%d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [28 x i8] c"Actual offsets: a=%d, b=%d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [23 x i8] c"VTable ptr intact: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [28 x i8] c"Copy VTable ptr intact: %d\0A\00", align 4
@printf.str.5 = private unnamed_addr constant [40 x i8] c"Outer offsets: a=%d, c=%d, member.a=%d\0A\00", align 4
@printf.str.6 = private unnamed_addr constant [25 x i8] c"Outer: a=%d, b=%d, c=%d\0A\00", align 4
@printf.str.7 = private unnamed_addr constant [34 x i8] c"Outer VTable ptrs intact: %d, %d\0A\00", align 4
@printf.str.8 = private unnamed_addr constant [27 x i8] c"Const literal: a=%d, b=%d\0A\00", align 4
@printf.str.9 = private unnamed_addr constant [21 x i8] c"Literal: a=%d, b=%d\0A\00", align 4

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN5Inner4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  store ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV5Inner, i64 0, i32 0, i32 2), ptr %2, align 8
  %3 = getelementptr inbounds nuw %struct.Inner, ptr %2, i32 0, i32 1
  store i32 0, ptr %3, align 4
  %4 = getelementptr inbounds nuw %struct.Inner, ptr %2, i32 0, i32 2
  store i64 0, ptr %4, align 8
  %5 = getelementptr inbounds nuw %struct.Inner, ptr %2, i32 0, i32 3
  store ptr null, ptr %5, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN5Inner4ctorERK5Inner(ptr noundef nonnull align 8 dereferenceable(32) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  %4 = load ptr, ptr %1, align 8
  store ptr %4, ptr %3, align 8
  %5 = getelementptr inbounds nuw %struct.Inner, ptr %1, i32 0, i32 1
  %6 = getelementptr inbounds nuw %struct.Inner, ptr %3, i32 0, i32 1
  call void @llvm.memcpy.p0.p0.i64(ptr %6, ptr %5, i64 4, i1 false)
  %7 = getelementptr inbounds nuw %struct.Inner, ptr %1, i32 0, i32 2
  %8 = getelementptr inbounds nuw %struct.Inner, ptr %3, i32 0, i32 2
  call void @llvm.memcpy.p0.p0.i64(ptr %8, ptr %7, i64 8, i1 false)
  %9 = getelementptr inbounds nuw %struct.Inner, ptr %1, i32 0, i32 3
  %10 = getelementptr inbounds nuw %struct.Inner, ptr %3, i32 0, i32 3
  %11 = load ptr, ptr %9, align 8
  store ptr null, ptr %10, align 8
  %12 = icmp ne ptr %11, null
  br i1 %12, label %nullptrcheck.then, label %nullptrcheck.exit

nullptrcheck.then:                                ; preds = %2
  %13 = call ptr @_Z12sAllocUnsafem(i64 4)
  store ptr %13, ptr %10, align 8
  call void @llvm.memcpy.p0.p0.i64(ptr %13, ptr %11, i64 4, i1 false)
  br label %nullptrcheck.exit

nullptrcheck.exit:                                ; preds = %nullptrcheck.then, %2
  ret void
}

; Function Attrs: nocallback nofree nosync nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias writeonly captures(none), ptr noalias readonly captures(none), i64, i1 immarg) #1

declare ptr @_Z12sAllocUnsafem(i64)

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN5Inner4ctorER5Inner(ptr noundef nonnull align 8 dereferenceable(32) %0, ptr %1) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %3 = load ptr, ptr %this, align 8
  %4 = load ptr, ptr %1, align 8
  store ptr %4, ptr %3, align 8
  %5 = getelementptr inbounds nuw %struct.Inner, ptr %1, i32 0, i32 1
  %6 = getelementptr inbounds nuw %struct.Inner, ptr %3, i32 0, i32 1
  call void @llvm.memcpy.p0.p0.i64(ptr %6, ptr %5, i64 4, i1 false)
  %7 = getelementptr inbounds nuw %struct.Inner, ptr %1, i32 0, i32 2
  %8 = getelementptr inbounds nuw %struct.Inner, ptr %3, i32 0, i32 2
  call void @llvm.memcpy.p0.p0.i64(ptr %8, ptr %7, i64 8, i1 false)
  %9 = getelementptr inbounds nuw %struct.Inner, ptr %1, i32 0, i32 3
  %10 = getelementptr inbounds nuw %struct.Inner, ptr %3, i32 0, i32 3
  %11 = load ptr, ptr %9, align 8
  store ptr %11, ptr %10, align 8
  store ptr null, ptr %9, align 8
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds nuw %struct.Inner, ptr %2, i32 0, i32 3
  call void @_Z8sDeallocRPVh(ptr %3)
  ret void
}

declare void @_Z8sDeallocRPVh(ptr)

; Function Attrs: noinline nounwind optnone uwtable
define internal void @_ZN5Inner5printEv(ptr noundef nonnull align 8 dereferenceable(32) %0) #2 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %a.addr = getelementptr inbounds %struct.Inner, ptr %2, i64 0, i32 1
  %3 = load i32, ptr %a.addr, align 4
  %4 = load ptr, ptr %this, align 8
  %b.addr = getelementptr inbounds %struct.Inner, ptr %4, i64 0, i32 2
  %5 = load i64, ptr %b.addr, align 8
  %6 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3, i64 noundef %5)
  ret void
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #3

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN5Outer4ctorEv(ptr noundef nonnull align 8 dereferenceable(72) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  call void @_ZN5Inner4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %2)
  %3 = getelementptr inbounds nuw %struct.Outer, ptr %2, i32 0, i32 1
  store i16 3, ptr %3, align 2
  %4 = getelementptr inbounds nuw %struct.Outer, ptr %2, i32 0, i32 2
  call void @_ZN5Inner4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %4)
  ret void
}

; Function Attrs: mustprogress noinline nounwind optnone uwtable
define void @_ZN5Outer4dtorEv(ptr noundef nonnull align 8 dereferenceable(72) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds nuw %struct.Outer, ptr %2, i32 0, i32 2
  call void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %3)
  %4 = load ptr, ptr %this, align 8
  call void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %4)
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define internal noundef zeroext i1 @_Z12hasVTablePtrRK5Inner(ptr noundef %0) #2 {
  %inner = alloca ptr, align 8
  %vtablePtrAddr = alloca ptr, align 8
  store ptr %0, ptr %inner, align 8
  %2 = load ptr, ptr %inner, align 8
  store ptr %2, ptr %vtablePtrAddr, align 8
  %3 = load ptr, ptr %vtablePtrAddr, align 8
  %4 = load ptr, ptr %3, align 8
  %5 = icmp ne ptr %4, null
  ret i1 %5
}

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define dso_local noundef i32 @main() #4 {
  %inner = alloca %struct.Inner, align 8
  %base = alloca i64, align 8
  %copy = alloca %struct.Inner, align 8
  %outer = alloca %struct.Outer, align 8
  %constLiteral = alloca %struct.Inner, align 8
  %ten = alloca i32, align 4
  %literal = alloca %struct.Inner, align 8
  call void @_ZN5Inner4ctorEv(ptr noundef nonnull align 8 dereferenceable(32) %inner)
  %a.addr = getelementptr inbounds %struct.Inner, ptr %inner, i64 0, i32 1
  store i32 1, ptr %a.addr, align 4
  %b.addr = getelementptr inbounds %struct.Inner, ptr %inner, i64 0, i32 2
  store i64 2, ptr %b.addr, align 8
  %1 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i64 noundef 8, i64 noundef 16, i64 noundef 24, i64 noundef 32)
  %2 = ptrtoint ptr %inner to i64
  store i64 %2, ptr %base, align 8
  %a.addr1 = getelementptr inbounds %struct.Inner, ptr %inner, i64 0, i32 1
  %3 = ptrtoint ptr %a.addr1 to i64
  %4 = load i64, ptr %base, align 8
  %5 = sub nsw i64 %3, %4
  %b.addr2 = getelementptr inbounds %struct.Inner, ptr %inner, i64 0, i32 2
  %6 = ptrtoint ptr %b.addr2 to i64
  %7 = load i64, ptr %base, align 8
  %8 = sub nsw i64 %6, %7
  %9 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i64 noundef %5, i64 noundef %8)
  %10 = call noundef zeroext i1 @_Z12hasVTablePtrRK5Inner(ptr noundef %inner)
  %11 = zext i1 %10 to i32
  %12 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %11)
  call void @_ZN5Inner5printEv(ptr noundef nonnull align 8 dereferenceable(32) %inner)
  call void @_ZN5Inner4ctorERK5Inner(ptr noundef nonnull align 8 dereferenceable(32) %copy, ptr %inner)
  %13 = call noundef zeroext i1 @_Z12hasVTablePtrRK5Inner(ptr noundef %copy)
  %14 = zext i1 %13 to i32
  %15 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %14)
  call void @_ZN5Inner5printEv(ptr noundef nonnull align 8 dereferenceable(32) %copy)
  call void @_ZN5Outer4ctorEv(ptr noundef nonnull align 8 dereferenceable(72) %outer)
  %a.addr3 = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 0, i32 1
  store i32 4, ptr %a.addr3, align 4
  %b.addr4 = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 0, i32 2
  store i64 5, ptr %b.addr4, align 8
  %member.addr = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 2
  %a.addr5 = getelementptr inbounds %struct.Inner, ptr %member.addr, i64 0, i32 1
  store i32 6, ptr %a.addr5, align 4
  %member.addr6 = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 2
  %b.addr7 = getelementptr inbounds %struct.Inner, ptr %member.addr6, i64 0, i32 2
  store i64 7, ptr %b.addr7, align 8
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, i64 noundef 8, i64 noundef 32, i64 noundef 48)
  %a.addr8 = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 0, i32 1
  %17 = load i32, ptr %a.addr8, align 4
  %b.addr9 = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 0, i32 2
  %18 = load i64, ptr %b.addr9, align 8
  %c.addr = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 1
  %19 = load i16, ptr %c.addr, align 2
  %20 = sext i16 %19 to i32
  %21 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.6, i32 noundef %17, i64 noundef %18, i32 noundef %20)
  %inner.addr = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 0
  %22 = call noundef zeroext i1 @_Z12hasVTablePtrRK5Inner(ptr noundef %inner.addr)
  %23 = zext i1 %22 to i32
  %member.addr10 = getelementptr inbounds %struct.Outer, ptr %outer, i64 0, i32 2
  %24 = call noundef zeroext i1 @_Z12hasVTablePtrRK5Inner(ptr noundef %member.addr10)
  %25 = zext i1 %24 to i32
  %26 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.7, i32 noundef %23, i32 noundef %25)
  %27 = getelementptr inbounds nuw %struct.Outer, ptr %outer, i32 0, i32 2
  call void @_ZN5Inner5printEv(ptr noundef nonnull align 8 dereferenceable(32) %27)
  store %struct.Inner { ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV5Inner, i64 0, i32 0, i32 2), i32 8, i64 9, ptr null }, ptr %constLiteral, align 8
  %a.addr11 = getelementptr inbounds %struct.Inner, ptr %constLiteral, i64 0, i32 1
  %28 = load i32, ptr %a.addr11, align 4
  %b.addr12 = getelementptr inbounds %struct.Inner, ptr %constLiteral, i64 0, i32 2
  %29 = load i64, ptr %b.addr12, align 8
  %30 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.8, i32 noundef %28, i64 noundef %29)
  store i32 10, ptr %ten, align 4
  store ptr getelementptr inbounds ({ [2 x ptr] }, ptr @_ZTV5Inner, i64 0, i32 0, i32 2), ptr %literal, align 8
  %31 = load i32, ptr %ten, align 4
  %32 = getelementptr inbounds nuw %struct.Inner, ptr %literal, i32 0, i32 1
  store i32 %31, ptr %32, align 4
  %33 = getelementptr inbounds nuw %struct.Inner, ptr %literal, i32 0, i32 2
  store i64 11, ptr %33, align 8
  %34 = getelementptr inbounds nuw %struct.Inner, ptr %literal, i32 0, i32 3
  store ptr null, ptr %34, align 8
  %a.addr13 = getelementptr inbounds %struct.Inner, ptr %literal, i64 0, i32 1
  %35 = load i32, ptr %a.addr13, align 4
  %b.addr14 = getelementptr inbounds %struct.Inner, ptr %literal, i64 0, i32 2
  %36 = load i64, ptr %b.addr14, align 8
  %37 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.9, i32 noundef %35, i64 noundef %36)
  call void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %literal)
  call void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %constLiteral)
  call void @_ZN5Outer4dtorEv(ptr noundef nonnull align 8 dereferenceable(72) %outer)
  call void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %copy)
  call void @_ZN5Inner4dtorEv(ptr noundef nonnull align 8 dereferenceable(32) %inner)
  ret i32 0
}

attributes #0 = { mustprogress noinline nounwind optnone uwtable }
attributes #1 = { nocallback nofree nosync nounwind willreturn memory(argmem: readwrite) }
attributes #2 = { noinline nounwind optnone uwtable }
attributes #3 = { nofree nounwind }
attributes #4 = { mustprogress noinline norecurse nounwind optnone uwtable }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev [host] (https://github.com/spicelang/spice)"}
