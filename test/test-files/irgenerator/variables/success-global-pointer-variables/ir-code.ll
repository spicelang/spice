; ModuleID = 'source.spice'
source_filename = "source.spice"

%struct.Registry = type { i32 }

@INT_PTR = internal global ptr null
@STRING_PTR = internal global ptr null
@printf.str.0 = private unnamed_addr constant [21 x i8] c"Registry is nil: %d\0A\00", align 4
@ACTIVE_REGISTRY = external global ptr
@printf.str.1 = private unnamed_addr constant [20 x i8] c"Int ptr is nil: %d\0A\00", align 4
@printf.str.2 = private unnamed_addr constant [13 x i8] c"Counter: %d\0A\00", align 4
@printf.str.3 = private unnamed_addr constant [13 x i8] c"Counter: %d\0A\00", align 4
@printf.str.4 = private unnamed_addr constant [11 x i8] c"Value: %d\0A\00", align 4
@anon.string.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 4
@printf.str.5 = private unnamed_addr constant [12 x i8] c"String: %s\0A\00", align 4

; Function Attrs: mustprogress noinline norecurse nounwind optnone uwtable
define noundef i32 @main() #0 {
  %registry = alloca %struct.Registry, align 8
  %value = alloca i32, align 4
  %str = alloca ptr, align 8
  %1 = load ptr, ptr @ACTIVE_REGISTRY, align 8
  %2 = icmp eq ptr %1, null
  %3 = zext i1 %2 to i32
  %4 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 noundef %3)
  %5 = load ptr, ptr @INT_PTR, align 8
  %6 = icmp eq ptr %5, null
  %7 = zext i1 %6 to i32
  %8 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 noundef %7)
  store %struct.Registry { i32 42 }, ptr %registry, align 4
  call void @_Z17setActiveRegistryP8Registry(ptr noundef align 4 %registry)
  %9 = load ptr, ptr @ACTIVE_REGISTRY, align 8
  %counter.addr = getelementptr inbounds %struct.Registry, ptr %9, i64 0, i32 0
  %10 = load i32, ptr %counter.addr, align 4
  %11 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.2, i32 noundef %10)
  %12 = load ptr, ptr @ACTIVE_REGISTRY, align 8
  %counter.addr1 = getelementptr inbounds %struct.Registry, ptr %12, i64 0, i32 0
  %13 = load i32, ptr %counter.addr1, align 4
  %14 = add nsw i32 %13, 1
  store i32 %14, ptr %counter.addr1, align 4
  %counter.addr2 = getelementptr inbounds %struct.Registry, ptr %registry, i64 0, i32 0
  %15 = load i32, ptr %counter.addr2, align 4
  %16 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.3, i32 noundef %15)
  store i32 7, ptr %value, align 4
  store ptr %value, ptr @INT_PTR, align 8
  %17 = load ptr, ptr @INT_PTR, align 8
  store i32 8, ptr %17, align 4
  %18 = load i32, ptr %value, align 4
  %19 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.4, i32 noundef %18)
  store ptr @anon.string.0, ptr %str, align 8
  store ptr %str, ptr @STRING_PTR, align 8
  %20 = load ptr, ptr @STRING_PTR, align 8
  %21 = load ptr, ptr %20, align 8
  %22 = call noundef i32 (ptr, ...) @printf(ptr noundef @printf.str.5, ptr noundef %21)
  ret i32 0
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr noundef readonly captures(none), ...) local_unnamed_addr #1

declare void @_Z17setActiveRegistryP8Registry(ptr)

attributes #0 = { mustprogress noinline norecurse nounwind optnone uwtable }
attributes #1 = { nofree nounwind }

!llvm.module.flags = !{!0, !1, !2, !3}
!llvm.ident = !{!4}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{!"spice version dev (https://github.com/spicelang/spice)"}
