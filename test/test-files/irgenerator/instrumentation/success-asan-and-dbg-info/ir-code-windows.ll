; ModuleID = 'source.spice'
source_filename = "source.spice"

@__asan_shadow_memory_dynamic_address = external global i64
@__asan_option_detect_stack_use_after_return = external global i32
@___asan_gen_stack = private unnamed_addr constant [16 x i8] c"1 32 8 6 iPtr:4\00", align 1
@llvm.used = appending global [1 x ptr] [ptr @asan.module_ctor], section "llvm.metadata"
@llvm.global_ctors = appending global [1 x { i32, ptr, ptr }] [{ i32, ptr, ptr } { i32 1, ptr @asan.module_ctor, ptr null }]

; Function Attrs: mustprogress noinline norecurse nounwind optnone sanitize_address uwtable
define noundef i32 @main() #0 !dbg !10 {
  %1 = load i64, ptr @__asan_shadow_memory_dynamic_address, align 8
  %asan_local_stack_base = alloca i64, align 8
  %2 = load i32, ptr @__asan_option_detect_stack_use_after_return, align 4
  %3 = icmp ne i32 %2, 0
  br i1 %3, label %4, label %6

4:                                                ; preds = %0
  %5 = call i64 @__asan_stack_malloc_0(i64 64)
  br label %6

6:                                                ; preds = %0, %4
  %7 = phi i64 [ 0, %0 ], [ %5, %4 ]
  %8 = inttoptr i64 %7 to ptr
  %9 = icmp eq i64 %7, 0
  br i1 %9, label %10, label %11

10:                                               ; preds = %6
  %MyAlloca = alloca i8, i64 64, align 32
  br label %11

11:                                               ; preds = %6, %10
  %12 = phi ptr [ %8, %6 ], [ %MyAlloca, %10 ]
  store ptr %12, ptr %asan_local_stack_base, align 8
  %13 = getelementptr i8, ptr %12, i64 32
  store i64 1102416563, ptr %12, align 8
  %14 = getelementptr i8, ptr %12, i64 8
  store i64 ptrtoint (ptr @___asan_gen_stack to i64), ptr %14, align 8
  %15 = getelementptr i8, ptr %12, i64 16
  store i64 ptrtoint (ptr @main to i64), ptr %15, align 8
  %16 = ptrtoint ptr %12 to i64
  %17 = lshr i64 %16, 3
  %18 = add i64 %17, %1
  %19 = add i64 %18, 0
  %20 = inttoptr i64 %19 to ptr
  store i64 -868082052615769615, ptr %20, align 1
  %21 = call ptr @_Z12sAllocUnsafem(i64 4), !dbg !17
  %22 = ptrtoint ptr %21 to i64, !dbg !17
  %23 = lshr i64 %22, 3, !dbg !17
  %24 = add i64 %23, %1, !dbg !17
  %25 = inttoptr i64 %24 to ptr, !dbg !17
  %26 = load i8, ptr %25, align 1, !dbg !17
  %27 = icmp ne i8 %26, 0, !dbg !17
  br i1 %27, label %28, label %34, !dbg !17, !prof !18

28:                                               ; preds = %11
  %29 = and i64 %22, 7, !dbg !17
  %30 = add i64 %29, 3, !dbg !17
  %31 = trunc i64 %30 to i8, !dbg !17
  %32 = icmp sge i8 %31, %26, !dbg !17
  br i1 %32, label %33, label %34, !dbg !17

33:                                               ; preds = %28
  call void @__asan_report_store4(i64 %22) #4, !dbg !17
  unreachable

34:                                               ; preds = %28, %11
  store i32 0, ptr %21, align 4, !dbg !17
  %35 = add i64 %18, 4, !dbg !17
  %36 = inttoptr i64 %35 to ptr, !dbg !17
  store i8 0, ptr %36, align 1, !dbg !17
    #dbg_declare(ptr %asan_local_stack_base, !15, !DIExpression(DW_OP_deref, DW_OP_plus_uconst, 32), !17)
  store ptr %21, ptr %13, align 8, !dbg !17
  %37 = load ptr, ptr %13, align 8, !dbg !19
  %38 = ptrtoint ptr %37 to i64, !dbg !20
  %39 = lshr i64 %38, 3, !dbg !20
  %40 = add i64 %39, %1, !dbg !20
  %41 = inttoptr i64 %40 to ptr, !dbg !20
  %42 = load i8, ptr %41, align 1, !dbg !20
  %43 = icmp ne i8 %42, 0, !dbg !20
  br i1 %43, label %44, label %50, !dbg !20, !prof !18

44:                                               ; preds = %34
  %45 = and i64 %38, 7, !dbg !20
  %46 = add i64 %45, 3, !dbg !20
  %47 = trunc i64 %46 to i8, !dbg !20
  %48 = icmp sge i8 %47, %42, !dbg !20
  br i1 %48, label %49, label %50, !dbg !20

49:                                               ; preds = %44
  call void @__asan_report_store4(i64 %38) #4, !dbg !20
  unreachable

50:                                               ; preds = %44, %34
  store i32 123, ptr %37, align 4, !dbg !20
  call void @_Z8sDeallocRPVh(ptr noundef %13), !dbg !21
  %51 = load ptr, ptr %13, align 8, !dbg !23
  %52 = ptrtoint ptr %51 to i64, !dbg !24
  %53 = lshr i64 %52, 3, !dbg !24
  %54 = add i64 %53, %1, !dbg !24
  %55 = inttoptr i64 %54 to ptr, !dbg !24
  %56 = load i8, ptr %55, align 1, !dbg !24
  %57 = icmp ne i8 %56, 0, !dbg !24
  br i1 %57, label %58, label %64, !dbg !24, !prof !18

58:                                               ; preds = %50
  %59 = and i64 %52, 7, !dbg !24
  %60 = add i64 %59, 3, !dbg !24
  %61 = trunc i64 %60 to i8, !dbg !24
  %62 = icmp sge i8 %61, %56, !dbg !24
  br i1 %62, label %63, label %64, !dbg !24

63:                                               ; preds = %58
  call void @__asan_report_store4(i64 %52) #4, !dbg !24
  unreachable

64:                                               ; preds = %58, %50
  store i32 321, ptr %51, align 4, !dbg !24
  call void @_Z8sDeallocRPVh(ptr %13), !dbg !25
  %65 = add i64 %18, 4, !dbg !25
  %66 = inttoptr i64 %65 to ptr, !dbg !25
  store i8 -8, ptr %66, align 1, !dbg !25
  store i64 1172321806, ptr %12, align 8, !dbg !25
  %67 = icmp ne i64 %7, 0, !dbg !25
  br i1 %67, label %68, label %74, !dbg !25

68:                                               ; preds = %64
  %69 = add i64 %18, 0, !dbg !25
  %70 = inttoptr i64 %69 to ptr, !dbg !25
  store i64 -723401728380766731, ptr %70, align 1, !dbg !25
  %71 = getelementptr i8, ptr %8, i64 56, !dbg !25
  %72 = load i64, ptr %71, align 8, !dbg !25
  %73 = inttoptr i64 %72 to ptr, !dbg !25
  store i8 0, ptr %73, align 1, !dbg !25
  br label %77, !dbg !25

74:                                               ; preds = %64
  %75 = add i64 %18, 0, !dbg !25
  %76 = inttoptr i64 %75 to ptr, !dbg !25
  store i64 0, ptr %76, align 1, !dbg !25
  br label %77, !dbg !25

77:                                               ; preds = %74, %68
  ret i32 0, !dbg !25
}

declare ptr @_Z12sAllocUnsafem(i64)

; Function Attrs: nobuiltin nocallback nofree nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, errnomem: none, target_mem: none)
declare void @llvm.lifetime.start.p0(ptr captures(none)) #1

declare void @_Z8sDeallocRPVh(ptr)

; Function Attrs: nobuiltin nocallback nofree nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, errnomem: none, target_mem: none)
declare void @llvm.lifetime.end.p0(ptr captures(none)) #1

declare void @__asan_report_load_n(i64, i64)

declare void @__asan_loadN(i64, i64)

declare void @__asan_report_load1(i64)

declare void @__asan_load1(i64)

declare void @__asan_report_load2(i64)

declare void @__asan_load2(i64)

declare void @__asan_report_load4(i64)

declare void @__asan_load4(i64)

declare void @__asan_report_load8(i64)

declare void @__asan_load8(i64)

declare void @__asan_report_load16(i64)

declare void @__asan_load16(i64)

declare void @__asan_report_store_n(i64, i64)

declare void @__asan_storeN(i64, i64)

declare void @__asan_report_store1(i64)

declare void @__asan_store1(i64)

declare void @__asan_report_store2(i64)

declare void @__asan_store2(i64)

declare void @__asan_report_store4(i64)

declare void @__asan_store4(i64)

declare void @__asan_report_store8(i64)

declare void @__asan_store8(i64)

declare void @__asan_report_store16(i64)

declare void @__asan_store16(i64)

declare void @__asan_report_exp_load_n(i64, i64, i32)

declare void @__asan_exp_loadN(i64, i64, i32)

declare void @__asan_report_exp_load1(i64, i32)

declare void @__asan_exp_load1(i64, i32)

declare void @__asan_report_exp_load2(i64, i32)

declare void @__asan_exp_load2(i64, i32)

declare void @__asan_report_exp_load4(i64, i32)

declare void @__asan_exp_load4(i64, i32)

declare void @__asan_report_exp_load8(i64, i32)

declare void @__asan_exp_load8(i64, i32)

declare void @__asan_report_exp_load16(i64, i32)

declare void @__asan_exp_load16(i64, i32)

declare void @__asan_report_exp_store_n(i64, i64, i32)

declare void @__asan_exp_storeN(i64, i64, i32)

declare void @__asan_report_exp_store1(i64, i32)

declare void @__asan_exp_store1(i64, i32)

declare void @__asan_report_exp_store2(i64, i32)

declare void @__asan_exp_store2(i64, i32)

declare void @__asan_report_exp_store4(i64, i32)

declare void @__asan_exp_store4(i64, i32)

declare void @__asan_report_exp_store8(i64, i32)

declare void @__asan_exp_store8(i64, i32)

declare void @__asan_report_exp_store16(i64, i32)

declare void @__asan_exp_store16(i64, i32)

declare ptr @__asan_memmove(ptr, ptr, i64)

declare ptr @__asan_memcpy(ptr, ptr, i64)

declare ptr @__asan_memset(ptr, i32, i64)

declare void @__asan_handle_no_return()

declare void @__sanitizer_ptr_cmp(i64, i64)

declare void @__sanitizer_ptr_sub(i64, i64)

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.shared(ptr) #2

; Function Attrs: nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none)
declare i1 @llvm.amdgcn.is.private(ptr) #2

declare i64 @__asan_stack_malloc_0(i64)

declare void @__asan_stack_free_0(i64, i64)

declare i64 @__asan_stack_malloc_1(i64)

declare void @__asan_stack_free_1(i64, i64)

declare i64 @__asan_stack_malloc_2(i64)

declare void @__asan_stack_free_2(i64, i64)

declare i64 @__asan_stack_malloc_3(i64)

declare void @__asan_stack_free_3(i64, i64)

declare i64 @__asan_stack_malloc_4(i64)

declare void @__asan_stack_free_4(i64, i64)

declare i64 @__asan_stack_malloc_5(i64)

declare void @__asan_stack_free_5(i64, i64)

declare i64 @__asan_stack_malloc_6(i64)

declare void @__asan_stack_free_6(i64, i64)

declare i64 @__asan_stack_malloc_7(i64)

declare void @__asan_stack_free_7(i64, i64)

declare i64 @__asan_stack_malloc_8(i64)

declare void @__asan_stack_free_8(i64, i64)

declare i64 @__asan_stack_malloc_9(i64)

declare void @__asan_stack_free_9(i64, i64)

declare i64 @__asan_stack_malloc_10(i64)

declare void @__asan_stack_free_10(i64, i64)

declare void @__asan_poison_stack_memory(i64, i64)

declare void @__asan_unpoison_stack_memory(i64, i64)

declare void @__asan_set_shadow_00(i64, i64)

declare void @__asan_set_shadow_01(i64, i64)

declare void @__asan_set_shadow_02(i64, i64)

declare void @__asan_set_shadow_03(i64, i64)

declare void @__asan_set_shadow_04(i64, i64)

declare void @__asan_set_shadow_05(i64, i64)

declare void @__asan_set_shadow_06(i64, i64)

declare void @__asan_set_shadow_07(i64, i64)

declare void @__asan_set_shadow_f1(i64, i64)

declare void @__asan_set_shadow_f2(i64, i64)

declare void @__asan_set_shadow_f3(i64, i64)

declare void @__asan_set_shadow_f5(i64, i64)

declare void @__asan_set_shadow_f8(i64, i64)

declare void @__asan_alloca_poison(i64, i64)

declare void @__asan_allocas_unpoison(i64, i64)

declare void @__asan_before_dynamic_init(i64)

declare void @__asan_after_dynamic_init()

declare void @__asan_register_globals(i64, i64)

declare void @__asan_unregister_globals(i64, i64)

declare void @__asan_register_image_globals(i64)

declare void @__asan_unregister_image_globals(i64)

declare void @__asan_register_elf_globals(i64, i64, i64)

declare void @__asan_unregister_elf_globals(i64, i64, i64)

declare void @__asan_init()

; Function Attrs: nounwind uwtable
define internal void @asan.module_ctor() #3 {
  call void @__asan_init()
  call void @__asan_version_mismatch_check_v8()
  ret void
}

declare void @__asan_version_mismatch_check_v8()

attributes #0 = { mustprogress noinline norecurse nounwind optnone sanitize_address uwtable }
attributes #1 = { nobuiltin nocallback nofree nosync nounwind willreturn memory(readwrite, inaccessiblemem: none, errnomem: none, target_mem: none) }
attributes #2 = { nocallback nocreateundeforpoison nofree nosync nounwind speculatable willreturn memory(none) }
attributes #3 = { nounwind uwtable }
attributes #4 = { nomerge }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5, !6}
!llvm.ident = !{!7}
!llvm.dbg.cu = !{!8}

!0 = !{i32 8, !"PIC Level", i32 2}
!1 = !{i32 7, !"PIE Level", i32 2}
!2 = !{i32 7, !"uwtable", i32 2}
!3 = !{i32 7, !"frame-pointer", i32 0}
!4 = !{i32 7, !"Dwarf Version", i32 5}
!5 = !{i32 2, !"Debug Info Version", i32 3}
!6 = !{i32 4, !"nosanitize_address", i32 1}
!7 = !{!"spice version dev [self-hosted] (https://github.com/spicelang/spice)"}
!8 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !9, producer: "spice version dev [self-hosted] (https://github.com/spicelang/spice)", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, splitDebugInlining: false)
!9 = !DIFile(filename: "source.spice", directory: "./test-files/irgenerator/instrumentation/success-asan-and-dbg-info")
!10 = distinct !DISubprogram(name: "main", linkageName: "_Z4mainv", scope: !9, file: !9, line: 3, type: !11, scopeLine: 3, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !8, retainedNodes: !14)
!11 = !DISubroutineType(types: !12)
!12 = !{!13}
!13 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!14 = !{!15}
!15 = !DILocalVariable(name: "iPtr", scope: !10, file: !9, line: 4, type: !16)
!16 = !DIDerivedType(tag: DW_TAG_pointer_type, baseType: !13, size: 64, dwarfAddressSpace: 0)
!17 = !DILocation(line: 4, column: 22, scope: !10)
!18 = !{!"branch_weights", i32 1, i32 1048575}
!19 = !DILocation(line: 5, column: 6, scope: !10)
!20 = !DILocation(line: 5, column: 13, scope: !10)
!21 = !DILocation(line: 7, column: 35, scope: !22)
!22 = distinct !DILexicalBlock(scope: !10, file: !9, line: 6, column: 5)
!23 = !DILocation(line: 9, column: 6, scope: !10)
!24 = !DILocation(line: 9, column: 13, scope: !10)
!25 = !DILocation(line: 10, column: 1, scope: !10)
