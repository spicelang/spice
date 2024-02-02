; ModuleID = 'source.spice'
source_filename = "source.spice"
target datalayout = "e-m:w-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-f80:128-n8:16:32:64-S128"
target triple = "x86_64-w64-windows-gnu"

%struct.StructToCopy = type { i32, i1 }

@printf.str.0 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1
@printf.str.1 = private unnamed_addr constant [7 x i8] c"%d %d\0A\00", align 1

; Function Attrs: norecurse
define void @_ZN12StructToCopy4ctorEv(ptr noundef nonnull align 4 dereferenceable(8) %0) #0 {
  %this = alloca ptr, align 8
  store ptr %0, ptr %this, align 8
  %2 = load ptr, ptr %this, align 8
  %3 = getelementptr inbounds %struct.StructToCopy, ptr %2, i32 0, i32 0
  store i32 21, ptr %3, align 4
  %4 = getelementptr inbounds %struct.StructToCopy, ptr %2, i32 0, i32 1
  store i1 false, ptr %4, align 1
  ret void
}

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #1 {
  %result = alloca i32, align 4
  %stc = alloca %struct.StructToCopy, align 8
  %stc2 = alloca %struct.StructToCopy, align 8
  store i32 0, ptr %result, align 4
  call void @_ZN12StructToCopy4ctorEv(ptr %stc)
  %a_addr = getelementptr inbounds %struct.StructToCopy, ptr %stc, i32 0, i32 0
  %1 = load i32, ptr %a_addr, align 4
  %b_addr = getelementptr inbounds %struct.StructToCopy, ptr %stc, i32 0, i32 1
  %2 = load i1, ptr %b_addr, align 1
  %3 = zext i1 %2 to i32
  %4 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.0, i32 %1, i32 %3)
  %5 = load %struct.StructToCopy, ptr %stc, align 4
  store %struct.StructToCopy %5, ptr %stc2, align 4
  %a_addr1 = getelementptr inbounds %struct.StructToCopy, ptr %stc2, i32 0, i32 0
  %6 = load i32, ptr %a_addr1, align 4
  %b_addr2 = getelementptr inbounds %struct.StructToCopy, ptr %stc2, i32 0, i32 1
  %7 = load i1, ptr %b_addr2, align 1
  %8 = zext i1 %7 to i32
  %9 = call i32 (ptr, ...) @printf(ptr noundef @printf.str.1, i32 %6, i32 %8)
  %10 = load i32, ptr %result, align 4
  ret i32 %10
}

; Function Attrs: nofree nounwind
declare noundef i32 @printf(ptr nocapture noundef readonly, ...) #2

attributes #0 = { norecurse }
attributes #1 = { noinline nounwind optnone uwtable }
attributes #2 = { nofree nounwind }
